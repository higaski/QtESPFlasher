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

/// QObject wrapped esp-serial-flasher
///
/// \file   esp_flasher.hpp
/// \author Vincent Hamp
/// \date   07/04/2024

#pragma once

#include <esp_loader.h>
#include <QSerialPort>
#include <QString>
#include <QWidget>
#include <array>
#include <chrono>
#include "available_ports.hpp"
#include "bin.hpp"
#include "options.hpp"

/// QObject wrapped esp-serial-flasher
///
/// EspFlasher wraps the esp-serial-flasher library into a singleton QObject.
class EspFlasher : public QObject,
                   public esp_loader_t,           // Context
                   public esp_loader_port_t,      // Port
                   public esp_loader_port_ops_t { // Port base
  Q_OBJECT

public:
  EspFlasher(QString chip,
             QString port,
             QString baud,
             QString before,
             QString after,
             QString no_stub,
             QString trace,
             QVector<Bin> bins = {});
  ~EspFlasher();

public slots:
  esp_loader_error_t flash();
  esp_loader_error_t erase();

signals:
  void finished();

private:
  QSerialPort::SerialPortError open(QString port = _port, QString baud = _baud);
  esp_loader_error_t connect(
    esp_loader_connect_args_t connect_config = ESP_LOADER_CONNECT_DEFAULT());
  esp_loader_error_t flash(Bin const& bin);
  esp_loader_error_t changeBaudRate();

  // Callbacks for esp_loader_port_ops_t
  static void loader_port_enter_bootloader(esp_loader_port_t* port);
  static void loader_port_reset_target(esp_loader_port_t* port);
  static void loader_port_start_timer(esp_loader_port_t*, uint32_t ms);
  static uint32_t loader_port_remaining_time(esp_loader_port_t*);
  static void loader_port_delay_ms(esp_loader_port_t*, uint32_t ms);
  static void loader_port_log(esp_loader_port_t*,
                              esp_loader_log_level_t level,
                              char const* fmt,
                              va_list args);
  static void loader_port_log_hex(esp_loader_port_t*,
                                  esp_loader_log_level_t level,
                                  char const* label,
                                  uint8_t const* data,
                                  size_t size);
  static esp_loader_error_t
  loader_port_change_transmission_rate(esp_loader_port_t*, uint32_t rate);
  static esp_loader_error_t loader_port_write(esp_loader_port_t*,
                                              uint8_t const* data,
                                              uint16_t size,
                                              uint32_t timeout);
  static esp_loader_error_t loader_port_read(esp_loader_port_t*,
                                             uint8_t* data,
                                             uint16_t size,
                                             uint32_t timeout);

  static inline std::unique_ptr<QSerialPort> _serial;
  static inline QString _chip;
  static inline QString _port;
  static inline QString _baud;
  static inline QString _before;
  static inline QString _after;
  static inline QString _no_stub;
  static inline QString _trace;
  static inline QVector<Bin> _bins;
  static inline std::chrono::time_point<std::chrono::steady_clock> _time_end;

  static constexpr std::array _level_prefix{"", "E", "W", "I", "D"};
  static constexpr int _default_baud{115200};
  static constexpr int _higher_baud{460800};

  // ESP32R0 has way longer times (1300u / 450u)?
  // Might need to make them changeable
  static_assert(SERIAL_FLASHER_RESET_HOLD_TIME_MS == 100);
  static_assert(SERIAL_FLASHER_BOOT_HOLD_TIME_MS == 50);
  static inline uint32_t _reset_hold_time_ms{SERIAL_FLASHER_RESET_HOLD_TIME_MS};
  static inline uint32_t _boot_hold_time_ms{SERIAL_FLASHER_BOOT_HOLD_TIME_MS};
};
