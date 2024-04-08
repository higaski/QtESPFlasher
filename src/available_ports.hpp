// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// Available ports
///
/// \file   available_ports.hpp
/// \author Vincent Hamp
/// \date   29/03/2024

#pragma once

#include <QSerialPortInfo>

QList<QSerialPortInfo> available_ports();