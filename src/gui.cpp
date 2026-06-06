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

/// GUI
///
/// \file   gui.cpp
/// \author Vincent Hamp
/// \date   29/03/2024

#include "gui.hpp"
#include <QFile>
#include <QFontDatabase>
#include "main_window.hpp"

/// Run GUI
///
/// \param  app Application instance
/// \return ​0​ if the GUI succeeds, nonzero value otherwise
int gui(QApplication& app) {
  // Initialize resources
  Q_INIT_RESOURCE(qtbreeze_stylesheets);

  /// \bug Adding DejaVuSansMono does not work on Windows
  if (QFontDatabase::addApplicationFont(":/fonts/DejaVuSansMono.ttf") == -1)
    return -1;
  QFont font{"DejaVuSansMono"};
  font.setPointSize(10);
  font.setStyleHint(QFont::Monospace);
  app.setFont(font);

  // Apply breeze stylesheet
  QFile file{":/dark/stylesheet.qss"};
  file.open(QFile::ReadOnly | QFile::Text);
  QTextStream stream{&file};
  app.setStyleSheet(stream.readAll());

  MainWindow w{};
  w.show();
  return app.exec();
}
