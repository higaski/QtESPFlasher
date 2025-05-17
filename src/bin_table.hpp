// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// Binary table
///
/// \file   bin_table.hpp
/// \author Vincent Hamp
/// \date   29/03/2024

#pragma once

#include <QTableWidget>
#include <esp_flasher/bin.hpp>

/// Table of binary paths and offsets
///
/// BinTable shows selected binary paths and offsets in a table. Binaries can
/// either be added by selecting an entire folder containing a
/// "flasher_args.json" file or by picking a single "flasher_args.json" file,
/// individual binaries or a .zip archive. A validation method is run every time
/// any of the cells changes. Only validated binaries are emitted through the
/// binaries signal.
class BinTable : public QTableWidget {
  Q_OBJECT

public:
  explicit BinTable(QWidget* parent = nullptr);

public slots:
  void addDirectory();
  void addFiles();
  void removeFiles();

signals:
  void binaries(QVector<Bin> bins);

private:
  static inline QColor const _red{255, 0, 0, 127 / 2};
  static inline QColor const _green{0, 255 / 2, 0, 127 / 2};

  void addDir(QString dir_path);
  void addBin(QString bin_path, QString offset = "");
  void addJson(QString json_path);
  void addAr(QString ar_path);

  void validate(int row, int column);
  void readBinaries();
};
