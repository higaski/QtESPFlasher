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
/// \file   update_ports_event_filter.cpp
/// \author Vincent Hamp
/// \date   29/03/2024

#include "update_ports_event_filter.hpp"
#include <QMouseEvent>
#include <esp_flasher/available_ports.hpp>

UpdatePortsEventFilter::UpdatePortsEventFilter(QWidget* parent)
  : QObject{parent} {}

bool UpdatePortsEventFilter::eventFilter(QObject* obj, QEvent* event) {
  if (event->type() == QEvent::MouseButtonPress &&
      static_cast<QMouseEvent*>(event)->button() == Qt::LeftButton) {
    QComboBox* port_combobox{static_cast<QComboBox*>(obj)};

    // Backup current port
    auto const current_port{port_combobox->currentText()};

    // Clear and create new list
    port_combobox->clear();
    for (auto const& port_info : available_ports())
      port_combobox->addItem(port_info.portName());
    port_combobox->addItem("auto");

    // Try to find backed up port
    auto const current_index{port_combobox->findText(current_port)};
    if (current_index != -1) port_combobox->setCurrentIndex(current_index);
  }
  // Standard event processing
  return QObject::eventFilter(obj, event);
}
