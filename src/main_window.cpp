// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

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
                        "\n\n" +       //
                        "@GitHub\n" +  //
                        "github.com/higaski/QtESPFlasher");
  about.setStandardButtons(QMessageBox::Ok);
  about.setIconPixmap(QPixmap{":/images/logo.png"});
  about.show();
  about.exec();
}
