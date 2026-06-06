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

/// Message handler
///
/// \file   message_handler.hpp
/// \author Vincent Hamp
/// \date   29/03/2024

#pragma once

#include <QObject>

/// Installs a Qt message handler
///
/// MessageHandler installs a (global) Qt message handler and emits all incoming
/// messages through its messageHandler signal.
class MessageHandler : public QObject {
  Q_OBJECT

public:
  static MessageHandler* get();

signals:
  void messageHandler(QtMsgType type,
                      QMessageLogContext const& context,
                      QString const& msg);

private:
  MessageHandler();
  ~MessageHandler();
  MessageHandler(MessageHandler const&) = delete;
  MessageHandler(MessageHandler&&) = delete;
  MessageHandler& operator=(MessageHandler const&) = delete;
  MessageHandler& operator=(MessageHandler&&) = delete;
};
