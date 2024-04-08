// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// GUI
///
/// \file   gui.cpp
/// \author Vincent Hamp
/// \date   29/03/2024

#include "gui.hpp"
#include <QFile>
#include "main_window.hpp"

/// Run GUI
///
/// \param  app Application instance
/// \return ​0​ if the GUI succeeds, nonzero value otherwise
int gui(QApplication& app) {
  Q_INIT_RESOURCE(qtbreeze_stylesheets);
  QFile file{":/dark/stylesheet.qss"};
  file.open(QFile::ReadOnly | QFile::Text);
  QTextStream stream{&file};
  app.setStyleSheet(stream.readAll());
  MainWindow w{};
  w.show();
  return app.exec();
}