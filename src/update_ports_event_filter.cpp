// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

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