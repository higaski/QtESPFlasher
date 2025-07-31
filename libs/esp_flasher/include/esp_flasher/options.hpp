// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

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
                                  "esp32h4",
                                  "esp32h2",
                                  "esp32c6",
                                  "esp32p4",
                                  "auto"};

/// Supported --before options
inline constexpr std::array befores{"default_reset", "no_reset"};

/// Supported --after options
inline constexpr std::array afters{"hard_reset", "no_reset"};
