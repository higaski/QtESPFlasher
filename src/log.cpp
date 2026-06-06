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
