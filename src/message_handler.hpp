// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

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
