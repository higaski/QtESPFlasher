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
