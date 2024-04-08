// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// GUI main window
///
/// \file   main_window.hpp
/// \author Vincent Hamp
/// \date   29/03/2024

#pragma once

#include <QMainWindow>
#include <QToolBar>
#include "bin_table.hpp"
#include "com_box.hpp"
#include "log.hpp"

/// Main window
class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow();

private slots:
  void about();

private:
  QToolBar* _toolbar{addToolBar("")};
  BinTable* _bin_table{new BinTable};
  ComBox* _com_box{new ComBox};
  Log* _log{new Log};
};
