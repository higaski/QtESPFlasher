// Copyright (C) 2024 Vincent Hamp
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

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
