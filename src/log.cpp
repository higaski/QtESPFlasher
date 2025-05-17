// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// GUI log
///
/// \file   log.cpp
/// \author Vincent Hamp
/// \date   29/03/2024

#include "log.hpp"
#include <QContextMenuEvent>
#include <QMenu>
#include "message_handler.hpp"

/// Connect QTextEdit to message handler
Log::Log(QWidget* parent) : QTextEdit{parent} {
  connect(MessageHandler::get(),
          &MessageHandler::messageHandler,
          this,
          &Log::messageHandler);
}

/// Append all incoming messages
void Log::messageHandler(QtMsgType type,
                         QMessageLogContext const& context,
                         QString const& msg) {
  switch (type) {
    case QtDebugMsg: append(msg); break;
    case QtInfoMsg: append(msg); break;
    case QtWarningMsg: append(msg); break;
    case QtCriticalMsg: append(msg); break;
    case QtFatalMsg: append(msg); break;
  }
}

/// Use standard context menu but delete a bunch of options
void Log::contextMenuEvent(QContextMenuEvent* event) {
  auto menu{createStandardContextMenu()};
  auto const actions{menu->actions()};
  menu->removeAction(actions[0u]); // Undo
  menu->removeAction(actions[1u]); // Redo
  menu->removeAction(actions[2u]); // Separator
  menu->removeAction(actions[5u]); // Paste
  menu->exec(event->globalPos());
  delete menu;
}

/// Disable pasting
void Log::insertFromMimeData(QMimeData const*) {}

/// Disable typing
void Log::keyPressEvent(QKeyEvent*) {}
