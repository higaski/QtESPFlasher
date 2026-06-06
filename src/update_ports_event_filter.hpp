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

/// Event filter for serial ports dropdown
///
/// \file   update_ports_event_filter.hpp
/// \author Vincent Hamp
/// \date   29/03/2024

#pragma once

#include <QComboBox>
#include <QEvent>

/// Event filter to update QComboBox widget which lists serial ports
///
/// UpdatePortsEventFilter is an event filter which can be installed on a
/// QComboBox widget which contains available serial port names. Installing this
/// filter allows updating the list of serial ports each time the QComboBox gets
/// altered.
class UpdatePortsEventFilter : public QObject {
  Q_OBJECT

public:
  explicit UpdatePortsEventFilter(QWidget* parent = nullptr);

protected:
  bool eventFilter(QObject* obj, QEvent* event) override;
};
