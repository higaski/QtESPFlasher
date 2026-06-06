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

/// GUI main window
///
/// \file   main_window.cpp
/// \author Vincent Hamp
/// \date   29/03/2024

#include "main_window.hpp"
#include <QApplication>
#include <QLabel>
#include <QMessageBox>
#include <QVBoxLayout>

/// Add menu and toolbar
MainWindow::MainWindow() {
  // Initial size
  resize(640, 640);

  // Toolbar should be fixed
  _toolbar->setContextMenuPolicy(Qt::PreventContextMenu);
  _toolbar->setFloatable(false);
  _toolbar->setMovable(false);

  // Add directory action
  QIcon const open_dir_icon{":/dark/folder_open.svg"};
  auto open_dir_act{new QAction{open_dir_icon, "Add directory", this}};
  connect(
    open_dir_act, &QAction::triggered, _bin_table, &BinTable::addDirectory);
  _toolbar->addAction(open_dir_act);

  // Add files action
  QIcon const open_icon{":/dark/file.svg"};
  auto open_act{new QAction{open_icon, "Add binaries", this}};
  connect(open_act, &QAction::triggered, _bin_table, &BinTable::addFiles);
  _toolbar->addAction(open_act);

  // Remove files action
  QIcon const remove_icon{":/dark/trash.svg"};
  auto remove_act{new QAction{remove_icon, "Remove binaries", this}};
  connect(remove_act, &QAction::triggered, _bin_table, &BinTable::removeFiles);
  _toolbar->addAction(remove_act);

  // Help action
  QIcon const about_icon{":/dark/dialog_help.svg"};
  auto about_act{new QAction{about_icon, "&About", this}};
  connect(about_act, &QAction::triggered, this, &MainWindow::about);
  _toolbar->addAction(about_act);

  // Layout
  auto central_widget{new QWidget};
  auto layout{new QVBoxLayout};
  layout->addWidget(_bin_table);
  layout->addWidget(_log);
  layout->addWidget(_com_box);
  layout->setStretchFactor(_bin_table, 3);
  layout->setStretchFactor(_log, 1);
  layout->setStretchFactor(_com_box, 0);
  central_widget->setLayout(layout);
  setCentralWidget(central_widget);

  connect(_bin_table, &BinTable::binaries, _com_box, &ComBox::binaries);
}

/// About message box
void MainWindow::about() {
  QMessageBox about;
  about.setWindowTitle("About " + QCoreApplication::applicationName());
  about.setTextFormat(Qt::RichText);
  about.setDetailedText("@Version\n" + QCoreApplication::applicationVersion() +
                        "\n\n" +      //
                        "@GitHub\n" + //
                        "github.com/higaski/QtESPFlasher");
  about.setStandardButtons(QMessageBox::Ok);
  about.setIconPixmap(QPixmap{":/images/logo.png"});
  about.show();
  about.exec();
}
