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
