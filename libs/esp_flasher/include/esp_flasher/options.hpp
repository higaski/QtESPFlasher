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

/// Options
///
/// \file   options.hpp
/// \author Vincent Hamp
/// \date   29/03/2024

#pragma once

#include <array>

/// Supported --chip options
inline constexpr std::array chips{"esp8266",
                                  "esp32",
                                  "esp32s2",
                                  "esp32c3",
                                  "esp32s3",
                                  "esp32c2",
                                  "esp32c5",
                                  "esp32h2",
                                  "esp32c6",
                                  "esp32p4",
                                  "auto"};

/// Supported --before options
inline constexpr std::array befores{"default_reset", "no_reset"};

/// Supported --after options
inline constexpr std::array afters{"hard_reset", "no_reset"};
