// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

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
#include "bin.hpp"

/// QObject wrapped esp-serial-flasher
///
/// EspFlasher wraps the esp-serial-flasher library into a singleton QObject.
/// Currently EspFlasher only supports flashing binaries.
class EspFlasher : public QObject {
  Q_OBJECT

public:
  EspFlasher(QString chip,
             QString port,
             QString baud,
             QString before,
             QString after,
             QString no_stub,
             QString trace,
             QVector<Bin> bins);
  ~EspFlasher();

public slots:
  esp_loader_error_t flash();

signals:
  void finished();

private:
  static QSerialPort::SerialPortError open(QString port = _port,
                                           QString baud = _baud);
  static esp_loader_error_t connect(
    esp_loader_connect_args_t connect_config = ESP_LOADER_CONNECT_DEFAULT());
  static esp_loader_error_t flash(Bin const& bin);
  static esp_loader_error_t changeBaudRate();

  // Mandatory
  static esp_loader_error_t loader_port_read(
    uint8_t* data, uint16_t size, uint32_t timeout) asm("loader_port_read");
  static esp_loader_error_t
  loader_port_write(uint8_t const* data,
                    uint16_t size,
                    uint32_t timeout) asm("loader_port_write");
  static void
  loader_port_enter_bootloader() asm("loader_port_enter_bootloader");
  static void loader_port_delay_ms(uint32_t ms) asm("loader_port_delay_ms");
  static void
  loader_port_start_timer(uint32_t ms) asm("loader_port_start_timer");
  static uint32_t
  loader_port_remaining_time() asm("loader_port_remaining_time");

  // Convenience
  static esp_loader_error_t loader_port_change_transmission_rate(
    uint32_t baudrate) asm("loader_port_change_transmission_rate");
  static void loader_port_reset_target() asm("loader_port_reset_target");
  static void
  loader_port_debug_print(char const* str) asm("loader_port_debug_print");

  static void
  transfer_debug_print(uint8_t const* data, uint16_t size, bool write);

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

  static constexpr int _default_baud{115200};
  static constexpr int _higher_baud{460800};

  // ESP32R0 hard way longer times (1300u / 450u)?
  // Might need to make them changeable
  static_assert(SERIAL_FLASHER_RESET_HOLD_TIME_MS == 100);
  static_assert(SERIAL_FLASHER_BOOT_HOLD_TIME_MS == 50);
  static inline uint32_t _reset_hold_time_ms{SERIAL_FLASHER_RESET_HOLD_TIME_MS};
  static inline uint32_t _boot_hold_time_ms{SERIAL_FLASHER_BOOT_HOLD_TIME_MS};
};