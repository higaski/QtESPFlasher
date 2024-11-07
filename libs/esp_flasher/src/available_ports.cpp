// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// Available ports
///
/// \file   available_ports.cpp
/// \author Vincent Hamp
/// \date   29/03/2024

#include "available_ports.hpp"

/// Get available serial ports
///
/// The list returned by QSerialPortInfo needs to be filtered to not contain
/// virtual ports. See https://stackoverflow.com/a/24854612/5840652 for further
/// details.
///
/// \return List of available serial ports
QList<QSerialPortInfo> available_ports() {
  QList<QSerialPortInfo> retval;
  for (auto const& port_info : QSerialPortInfo::availablePorts())
    if (!port_info.description().isEmpty()) retval.push_back(port_info);
  return retval;
}