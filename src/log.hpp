// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// GUI log
///
/// \file   log.hpp
/// \author Vincent Hamp
/// \date   29/03/2024

#pragma once

#include <QTextEdit>

/// Redirect Qt logging types to QTextEdit
///
/// Log redirects all Qt logging types (qCritical, qDebug, qFatal, qInfo and
/// qWarning) to a QTextEdit widget.
class Log : public QTextEdit {
  Q_OBJECT

public:
  explicit Log(QWidget* parent = nullptr);

private slots:
  void messageHandler(QtMsgType type,
                      QMessageLogContext const& context,
                      QString const& msg);

private:
  void contextMenuEvent(QContextMenuEvent* event) final;
  void insertFromMimeData(QMimeData const*) final;
  void keyPressEvent(QKeyEvent*) final;
};
