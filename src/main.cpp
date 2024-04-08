// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// main
///
/// \file   main.cpp
/// \author Vincent Hamp
/// \date   29/03/2024

#include <QApplication>
#include "cli.hpp"
#include "gui.hpp"

int main(int argc, char* argv[]) {
  QCoreApplication::setApplicationName("QtESPFlasher");
  QCoreApplication::setApplicationVersion(QTESPFLASHER_VERSION);

  // Create an application instance
  QApplication app{argc, argv};

  // CLI or GUI
  return argc > 1 ? cli(app) : gui(app);
}
