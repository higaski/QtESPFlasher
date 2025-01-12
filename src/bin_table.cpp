// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// Binary table
///
/// \file   bin_table.cpp
/// \author Vincent Hamp
/// \date   29/03/2024

#include "bin_table.hpp"
#include <JlCompress.h>
#include <QDebug>
#include <QDirIterator>
#include <QFileDialog>
#include <QHeaderView>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTableWidgetItem>
#include <QTemporaryDir>

/// Create rows and columns
BinTable::BinTable(QWidget* parent) : QTableWidget{parent} {
  setColumnCount(3);
  setColumnWidth(2, 40);

  // No vertical header
  verticalHeader()->hide();

  // Disable signals on horizontal headers (don't allow column selection)
  horizontalHeader()->blockSignals(true);

  // Stretch
  horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
  horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
  horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);

  // Set header text
  setHorizontalHeaderItem(0, new QTableWidgetItem{"Binary"});
  setHorizontalHeaderItem(1, new QTableWidgetItem{"@"});
  setHorizontalHeaderItem(2, new QTableWidgetItem{"Offset"});

  // Validate entire table every time a cell changes
  connect(this, &QTableWidget::cellChanged, this, &BinTable::validate);
}

/// Add directory containing a "flasher_args.json" file
void BinTable::addDirectory() {
  auto const path{QFileDialog::getExistingDirectory(
    this,
    "Add directory",
    "",
    QFileDialog::ShowDirsOnly | QFileDialog::DontUseCustomDirectoryIcons)};
  if (QFileInfo{path}.exists()) addDir(path);
}

/// Add files with either .bin, .json or .zip suffix
void BinTable::addFiles() {
  auto const paths{QFileDialog::getOpenFileNames(
    this,
    "Add binaries, JSON or zip archive",
    "",
    "Binaries, JSON or zip archive (*.bin *.json *.zip)",
    nullptr,
    QFileDialog::DontUseCustomDirectoryIcons)};
  std::ranges::for_each(paths, [&](auto path) {
    if (QFileInfo const file_info{path}; !file_info.exists()) return;
    else if (auto const suffix{file_info.suffix()}; suffix == "bin")
      addBin(path);
    else if (suffix == "json") addJson(path);
    else addAr(path);
  });
}

/// Remove selected binaries
void BinTable::removeFiles() {
  for (int r{}; r < rowCount();) {
    if (item(r, 0)->checkState()) {
      removeRow(r);
      r = 0; // Start over, rowCount changed
    } else ++r;
  }

  // For some reason removing files does not trigger cellChanged
  readBinaries();
}

/// Add directory containing a "flasher_args.json" file
///
/// \param  dir_path  Directory path
void BinTable::addDir(QString dir_path) {
  QDirIterator it{dir_path};
  while (it.hasNext()) {
    auto const path{it.next()};
    auto const info{QFileInfo{path}};
    if (info.fileName() == "flasher_args.json") return addJson(path);
  }
  qWarning().noquote() << "No flasher_args.json file found";
}

/// Add binary
///
/// \param  bin_path  Binary path
/// \param  offset    Offset
void BinTable::addBin(QString bin_path, QString offset) {
  auto const row{rowCount()};
  setRowCount(row + 1);

  // Binary
  auto bin_widget{new QTableWidgetItem{bin_path}};
  bin_widget->setCheckState(Qt::Checked);
  bin_widget->setFlags(bin_widget->flags() & ~Qt::ItemIsSelectable);
  setItem(row, 0, bin_widget);

  // @
  auto at_widget{new QTableWidgetItem};
  at_widget->setFlags(at_widget->flags() &
                      ~(Qt::ItemIsSelectable | Qt::ItemIsEditable));
  setItem(row, 1, at_widget);

  // Offset
  auto offset_widget{new QTableWidgetItem{offset}};
  offset_widget->setFlags(offset_widget->flags() & ~Qt::ItemIsSelectable);
  setItem(row, 2, offset_widget);
}

/// Add "flasher_args.json" file
///
/// \param  json_path Json path
void BinTable::addJson(QString json_path) {
  QFile file{json_path};
  file.open(QIODevice::ReadOnly | QIODevice::Text);
  QJsonDocument doc{QJsonDocument::fromJson(file.readAll())};
  auto const flash_files{doc["flash_files"]};
  if (flash_files == QJsonValue::Undefined) return;
  QJsonObject obj{flash_files.toObject()};
  for (auto const& offset : obj.keys()) {
    if (!obj.value(offset).isString()) continue;
    auto const bin_path{obj.value(offset).toString()};
    addBin(QFileInfo{json_path}.dir().filePath(bin_path), offset);
  }
}

/// Add .zip file
///
/// \param  ar_path Archive path
void BinTable::addAr(QString ar_path) {
  // Create temporary directory
  QTemporaryDir temp_dir;
  if (!temp_dir.isValid()) {
    qCritical().noquote() << temp_dir.errorString();
    return;
  }

  // Keep the folder around
  temp_dir.setAutoRemove(false);

  // Extract files to temporary directory, then add it
  JlCompress::extractDir(ar_path, temp_dir.path());

  // If folder might contains single subfolder use it instead
  QDir const dir{temp_dir.path()};
  if (auto const entries{
        dir.entryList(QDir::NoDotAndDotDot | QDir::Files | QDir::Dirs)};
      entries.size() == 1)
    if (auto const sub_dir{dir.filePath(entries.first())};
        QFileInfo{sub_dir}.isDir())
      return addDir(sub_dir);

  return addDir(dir.path());
}

/// Validate cell
///
/// \param  row Row
/// \param  col Column
void BinTable::validate(int row, int col) {
  auto const i{item(row, col)};

  // Bin
  if (col == 0) {
    QFileInfo const file_info{i->text()};
    i->setBackground(file_info.exists() ? _green : _red);
  }
  // Don't care
  else if (col == 1)
    ;
  // Offset
  else if (col == 2) {
    bool ok{};
    i->text().toUInt(&ok, 0);
    i->setBackground(ok ? _green : _red);
  }

  readBinaries();
}

/// Read binaries and emit binaries signal
///
/// Yes, this is awfully inefficient... but who cares.
void BinTable::readBinaries() {
  QVector<Bin> bins{};

  for (int i{}; i < rowCount(); ++i) {
    // Path must be valid and checked
    auto const bin_path{item(i, 0)};
    if (!bin_path || bin_path->background() != _green ||
        bin_path->checkState() != Qt::Checked)
      continue;

    // Offset must be valid
    auto const offset{item(i, 2)};
    if (!offset || offset->background() != _green) continue;

    // Read file
    QFile file{bin_path->text()};
    file.open(QIODevice::ReadOnly);
    bins.push_back(
      {.offset = offset->text().toUInt(nullptr, 0), .bytes = file.readAll()});
  }

  emit binaries(bins);
}