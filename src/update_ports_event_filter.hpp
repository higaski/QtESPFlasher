// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

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
