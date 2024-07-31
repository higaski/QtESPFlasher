// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// QObject wrapped esp-serial-flasher
///
/// \file   esp_flasher.cpp
/// \author Vincent Hamp
/// \date   07/04/2024

#include "esp_flasher.hpp"
#include <esp_loader_io.h>
#include <QApplication>
#include <QDebug>
#include <QThread>
#include <algorithm>
#include <gsl/util>
#include <magic_enum.hpp>
#include <thread>
#include "available_ports.hpp"
#include "options.hpp"

#if defined(Q_OS_LINUX)
#  include <sys/ioctl.h>
#elif defined(Q_OS_WIN)
#  include <windows.h>
#endif

using namespace std::chrono;

#define ESP_RETURN_ON_INTERRUPTION_REQUESTED()                                 \
  if (QThread::currentThread()->isInterruptionRequested())                     \
  return ESP_LOADER_ERROR_FAIL

namespace {

template<typename To, typename From>
std::enable_if_t<sizeof(To) == sizeof(From) &&
                   std::is_trivially_copyable_v<From> &&
                   std::is_trivially_copyable_v<To>,
                 To>
bit_cast(From const& src) noexcept {
  static_assert(std::is_trivially_constructible_v<To>);
  To dst;
  std::memcpy(&dst, &src, sizeof(To));
  return dst;
}

/// Convert esp_loader_error_t to QString
///
/// \param  err Error
/// \return QString of esp_loader_error_t enumeration
QString qstrerr(esp_loader_error_t err) {
  return QString::fromStdString(std::string(magic_enum::enum_name(err)));
}

/// Align
///
/// \param  alignment Alignment
/// \param  value     Value to align
/// \return Aligned value
constexpr size_t align(size_t alignment, size_t value) {
  return value + (alignment - value % alignment) % alignment;
}

}  // namespace

/// Create EspFlasher
///
/// \param  chip    Target
/// \param  port    Serial port
/// \param  baud    Baud rate
/// \param  before  What to do before connecting to the chip
/// \param  after   What to do after flashing is finished
/// \param  no_stub Disable launching the flasher stub
/// \param  trace   Enable trace-level output
/// \param  bins    Binaries to flash
EspFlasher::EspFlasher(QString chip,
                       QString port,
                       QString baud,
                       QString before,
                       QString after,
                       QString no_stub,
                       QString trace,
                       QVector<Bin> bins) {
  _chip = chip;
  _port = port;
  _baud = baud;
  _before = before;
  _after = after;
  _no_stub = no_stub;
  _trace = trace;
  _bins = bins;
}

EspFlasher::~EspFlasher() {
  _serial.reset();
  _chip.clear();
  _port.clear();
  _baud.clear();
  _before.clear();
  _after.clear();
  _no_stub.clear();
  _trace.clear();
  _bins.clear();
  _time_end = {};

  // Make sure internal esp_stub_running flag is reset
  esp_loader_reset_target();
}

/// Flash all binaries
///
/// \retval ESP_LOADER_SUCCESS              Success
/// \retval ESP_LOADER_ERROR_FAIL           Unspecified error
/// \retval ESP_LOADER_ERROR_TIMEOUT        Timeout elapsed
/// \retval ESP_LOADER_ERROR_INVALID_PARAM  Invalid parameter
/// \retval ESP_LOADER_ERROR_INVALID_TARGET Connected target is invalid
esp_loader_error_t EspFlasher::flash() {
  gsl::final_action emit_finished{[this] { emit finished(); }};

  // Try to connect
  if (auto const err{connect()}; err != ESP_LOADER_SUCCESS) {
    qCritical().noquote().nospace()
      << "Cannot connect to target (" << qstrerr(err) << ")";
    return err;
  }

  // Change to higher baud rate
  if (_baud == "auto" && esp_loader_get_target() != ESP8266_CHIP)
    if (auto const err{changeBaudRate()}; err != ESP_LOADER_SUCCESS) {
      qCritical().noquote().nospace()
        << "Cannot change baud rate (" << qstrerr(err) << ")";
      return err;
    }

  // Invoke flash on all binaries
  for (auto const& bin : _bins)
    if (auto const err{flash(bin)}; err != ESP_LOADER_SUCCESS) {
      qCritical().noquote().nospace()
        << "Flash failed (" << qstrerr(err) << ")";
      return err;
    }

  if (_after == "hard_reset") {
    qInfo().noquote() << "Hard resetting via RTS pin...";
    loader_port_reset_target();
  }

  qInfo().noquote() << "Done";

  return ESP_LOADER_SUCCESS;
}

/// Change to higher baud rate
///
/// \retval ESP_LOADER_SUCCESS                Success
/// \retval ESP_LOADER_ERROR_TIMEOUT          Timeout
/// \retval ESP_LOADER_ERROR_INVALID_RESPONSE Internal error
/// \retval ESP_LOADER_ERROR_UNSUPPORTED_FUNC Unsupported on the target
esp_loader_error_t EspFlasher::changeBaudRate() {
  if (auto const err{
        _no_stub == "no-stub"
          ? esp_loader_change_transmission_rate(_higher_baud)
          : esp_loader_change_transmission_rate_stub(
              _baud == "auto" ? _default_baud : _baud.toInt(), _higher_baud)};
      err != ESP_LOADER_SUCCESS)
    return err;
  if (auto const err{loader_port_change_transmission_rate(_higher_baud)};
      err != ESP_LOADER_SUCCESS)
    return err;
  qInfo().noquote() << "Changing baud rate to " + QString::number(_higher_baud);
  return ESP_LOADER_SUCCESS;
}

/// Open serial port
///
/// DTR and RTS both low (true) after that
///
/// \param  port  Serial port
/// \param  baud  Baud rate
/// \return SerialPortError
QSerialPort::SerialPortError EspFlasher::open(QString port, QString baud) {
  _serial = std::make_unique<QSerialPort>();
  _serial->setPortName(port);
  if (!_serial->open(QIODeviceBase::ReadWrite)) return _serial->error();
  if (!_serial->setBaudRate(baud == "auto" ? _default_baud : baud.toInt()))
    return _serial->error();
  if (!_serial->setDataBits(QSerialPort::Data8)) return _serial->error();
  if (!_serial->setStopBits(QSerialPort::OneStop)) return _serial->error();
  if (!_serial->setParity(QSerialPort::NoParity)) return _serial->error();
  if (!_serial->setFlowControl(QSerialPort::SoftwareControl))
    return _serial->error();

  auto fd{_serial->handle()};

#if defined(Q_OS_LINUX)
  int flags;
  ioctl(fd, TIOCMGET, &flags);

  // DTR low, RTS low
  flags |= TIOCM_DTR | TIOCM_RTS;
  ioctl(fd, TIOCMSET, &flags);
  loader_port_delay_ms(_reset_hold_time_ms);
#elif defined(Q_OS_WIN)
  DCB dcb;
  GetCommState(fd, &dcb);

  // DTR low, RTS low
  dcb.fDtrControl = DTR_CONTROL_ENABLE;
  dcb.fRtsControl = RTS_CONTROL_ENABLE;
  SetCommState(fd, &dcb);
  loader_port_delay_ms(_reset_hold_time_ms);
#endif

  return QSerialPort::NoError;
}

/// Connect to target
///
/// \param  connect_config                  Timing parameters
/// \retval ESP_LOADER_SUCCESS              Success
/// \retval ESP_LOADER_ERROR_FAIL           Unspecified error
/// \retval ESP_LOADER_ERROR_TIMEOUT        Timeout elapsed
/// \retval ESP_LOADER_ERROR_INVALID_PARAM  Invalid parameter
/// \retval ESP_LOADER_ERROR_INVALID_TARGET Connected target is invalid
esp_loader_error_t
EspFlasher::connect(esp_loader_connect_args_t connect_config) {
  esp_loader_error_t err{ESP_LOADER_ERROR_FAIL};

  // Try all ports and see if one opens and connects
  if (auto const connect_func{_no_stub == "no-stub"
                                ? esp_loader_connect
                                : esp_loader_connect_with_stub};
      _port == "auto") {
    auto const port_infos{available_ports()};
    qInfo() << "Found" << port_infos.size() << "serial ports";
    for (auto const& port_info : port_infos) {
      ESP_RETURN_ON_INTERRUPTION_REQUESTED();
      qInfo().noquote() << "Connecting to" << port_info.portName();
      _port = port_info.portName();
      // Can't open port
      if (open(_port, _baud) != QSerialPort::NoError) continue;
      // Can't connect
      else if ((err = connect_func(&connect_config)) != ESP_LOADER_SUCCESS)
        continue;
      // Done
      break;
    }
  }
  // Port defined, this must work
  else {
    ESP_RETURN_ON_INTERRUPTION_REQUESTED();
    qInfo().noquote() << "Connecting to" << _port;
    // Can't open port
    if (open(_port, _baud) != QSerialPort::NoError) return err;
    // Can't connect
    else if ((err = connect_func(&connect_config)) != ESP_LOADER_SUCCESS)
      return err;
  }

  // No port with ESP found
  if (err != ESP_LOADER_SUCCESS) return err;

  // Target supported
  auto const target{esp_loader_get_target()};
  if (target >= ESP_UNKNOWN_CHIP ||
      target == ESP32_RESERVED0_CHIP)  // TODO remove this one day?
  {
    qCritical().noquote() << "Unknown chip";
    return ESP_LOADER_ERROR_UNSUPPORTED_CHIP;
  }

  // Not the target defined
  if (_chip != "auto" && _chip != chips[target]) {
    qCritical().noquote() << "Selected target" << _chip << ", but found"
                          << chips[target];
    return ESP_LOADER_ERROR_INVALID_RESPONSE;
  }
  //
  else {
    _chip = chips[target];
    qInfo().noquote() << "Found" << _chip;
  }

  ESP_RETURN_ON_INTERRUPTION_REQUESTED();

  return err;
}

/// Flash binary
///
/// \param  bin                             Binary
/// \retval ESP_LOADER_SUCCESS              Success
/// \retval ESP_LOADER_ERROR_FAIL           Unspecified error
/// \retval ESP_LOADER_ERROR_TIMEOUT        Timeout elapsed
/// \retval ESP_LOADER_ERROR_INVALID_PARAM  Invalid parameter
esp_loader_error_t EspFlasher::flash(Bin const& bin) {
  constexpr uint32_t block_size{1024u};

  qInfo().nospace()
    << "Flash will be erased from 0x" << Qt::hex << bin.offset << " to 0x"
    << align(4096u, static_cast<size_t>(bin.offset + bin.bytes.size())) - 1u;
  if (auto const err{esp_loader_flash_start(
        bin.offset, static_cast<uint32_t>(bin.bytes.size()), block_size)};
      err != ESP_LOADER_SUCCESS) {
    qCritical().noquote().nospace()
      << "Erasing flash failed (" << qstrerr(err) << ")";
    return err;
  }

  ESP_RETURN_ON_INTERRUPTION_REQUESTED();

  auto first{bin.bytes.cbegin()};
  auto const last{bin.bytes.cend()};
  auto progress_it{first};
  auto const progress_step_size{bin.bytes.size() / 10};
  while (first < last) {
    // Write flash
    QByteArray payload(block_size, -1);
    auto const to_read{
      std::min(static_cast<uint32_t>(last - first), block_size)};
    std::copy_n(first, to_read, payload.begin());
    if (auto const err{esp_loader_flash_write(payload.data(), to_read)};
        err != ESP_LOADER_SUCCESS) {
      qCritical().noquote().nospace()
        << "Writing flash failed (" << qstrerr(err) << ")";
      return err;
    } else first += to_read;

    // Write progress to log
    if (first > progress_it) {
      auto const written{first - bin.bytes.cbegin()};
      progress_it += progress_step_size;
      auto const pct{100.0 * static_cast<double>(written) /
                     static_cast<double>(bin.bytes.size())};
      qInfo().nospace() << "Writing at 0x" << Qt::hex << bin.offset + written
                        << "... (" << Qt::dec << static_cast<int>(pct) << "%)";
    }

    ESP_RETURN_ON_INTERRUPTION_REQUESTED();
  }

#if MD5_ENABLED
  if (auto const err{esp_loader_flash_verify()};
      err == ESP_LOADER_ERROR_UNSUPPORTED_FUNC)
    printf("ESP8266 does not support flash verify command.");
  else if (err != ESP_LOADER_SUCCESS) {
    printf("MD5 does not match. err: %d\n", err);
    return err;
  }
  printf("Flash verified\n");
#endif

  return ESP_LOADER_SUCCESS;
}

/// Reads data from the io interface
///
/// \param  data                      Buffer for received data
/// \param  size                      Number of bytes to read
/// \param  timeout                   Timeout in milliseconds
/// \retval ESP_LOADER_SUCCESS        Success
/// \retval ESP_LOADER_ERROR_TIMEOUT  Timeout elapsed
esp_loader_error_t
EspFlasher::loader_port_read(uint8_t* data, uint16_t size, uint32_t timeout) {
  assert(_serial && _serial->isOpen());
  auto const then{steady_clock::now() + milliseconds{timeout}};
  while (_serial->bytesAvailable() < size) {
    _serial->waitForReadyRead(0);
    if (steady_clock::now() >= then) return ESP_LOADER_ERROR_TIMEOUT;
  }
  if (_serial->read(bit_cast<char*>(data), size) != size)
    return ESP_LOADER_ERROR_FAIL;
  else {
    transfer_debug_print(data, size, false);
    return ESP_LOADER_SUCCESS;
  }
}

/// Writes data over the io interface.
///
/// \param  data                      Buffer with data to be written
/// \param  size                      Size of data in bytes
/// \param  timeout                   Timeout in milliseconds
/// \retval ESP_LOADER_SUCCESS        Success
/// \retval ESP_LOADER_ERROR_TIMEOUT  Timeout elapsed
esp_loader_error_t EspFlasher::loader_port_write(uint8_t const* data,
                                                 uint16_t size,
                                                 uint32_t timeout) {
  assert(_serial && _serial->isOpen());
  auto const then{steady_clock::now() + milliseconds{timeout}};
  if (_serial->write(bit_cast<char*>(data), size) != size)
    return ESP_LOADER_ERROR_FAIL;
  while (_serial->bytesToWrite()) {
    _serial->waitForBytesWritten(0);
    if (steady_clock::now() >= then) return ESP_LOADER_ERROR_TIMEOUT;
  }
  transfer_debug_print(data, size, true);
  return ESP_LOADER_SUCCESS;
}

/// Asserts bootstrap pins to enter boot mode and toggles reset pin
void EspFlasher::loader_port_enter_bootloader() {
  if (_before != "default_reset") return;
  assert(_serial && _serial->isOpen());

  auto fd{_serial->handle()};

#if defined(Q_OS_LINUX)
  int flags;
  ioctl(fd, TIOCMGET, &flags);

  // DTR high, RTS low
  flags &= ~(TIOCM_DTR | TIOCM_RTS);
  flags |= TIOCM_RTS;
  ioctl(fd, TIOCMSET, &flags);
  loader_port_delay_ms(_reset_hold_time_ms);

  // DTR low, RTS high
  flags &= ~(TIOCM_DTR | TIOCM_RTS);
  flags |= TIOCM_DTR;
  ioctl(fd, TIOCMSET, &flags);
  loader_port_delay_ms(_boot_hold_time_ms);

  // DTR low, RTS low
  flags &= ~(TIOCM_DTR | TIOCM_RTS);
  flags |= TIOCM_DTR | TIOCM_RTS;
  ioctl(fd, TIOCMSET, &flags);

// Windows only propagates DTR on RTS setting!
#elif defined(Q_OS_WIN)
  DCB dcb;
  GetCommState(fd, &dcb);

  // DTR high, RTS low
  dcb.fDtrControl = DTR_CONTROL_DISABLE;
  dcb.fRtsControl = RTS_CONTROL_ENABLE;
  SetCommState(fd, &dcb);
  loader_port_delay_ms(_reset_hold_time_ms);

  // DTR low, RTS high
  dcb.fDtrControl = DTR_CONTROL_ENABLE;
  dcb.fRtsControl = RTS_CONTROL_DISABLE;
  SetCommState(fd, &dcb);
  loader_port_delay_ms(_boot_hold_time_ms);

  // DTR low, RTS low
  dcb.fDtrControl = DTR_CONTROL_ENABLE;
  dcb.fRtsControl = RTS_CONTROL_ENABLE;
  SetCommState(fd, &dcb);
#endif
}

/// Delay in milliseconds
///
/// \param  ms  Number of milliseconds
void EspFlasher::loader_port_delay_ms(uint32_t ms) {
  std::this_thread::sleep_for(milliseconds{ms});
}

/// Starts timeout timer
///
/// \param  ms  Number of milliseconds
void EspFlasher::loader_port_start_timer(uint32_t ms) {
  _time_end = steady_clock::now() + milliseconds{ms};
}

/// Returns remaining time since timer was started by calling
/// esp_loader_start_timer or 0 if timer has elapsed.
///
/// \return Number of milliseconds
uint32_t EspFlasher::loader_port_remaining_time() {
  auto const remaining{
    duration_cast<milliseconds>(_time_end - steady_clock::now())};
  return remaining > milliseconds::zero()
           ? static_cast<uint32_t>(remaining.count())
           : 0u;
}

/// Toggles reset pin
void EspFlasher::loader_port_reset_target() {
  if (_after != "hard_reset") return;
  assert(_serial && _serial->isOpen());
  _serial->setDataTerminalReady(false);
  loader_port_delay_ms(_reset_hold_time_ms);
  _serial->setDataTerminalReady(true);
}

/// Change baud rate
///
/// \param  baudrate              Baud rate
/// \retval ESP_LOADER_SUCCESS    Success
/// \retval ESP_LOADER_ERROR_FAIL Failure
esp_loader_error_t
EspFlasher::loader_port_change_transmission_rate(uint32_t baudrate) {
  assert(_serial && _serial->isOpen());
  return _serial->setBaudRate(static_cast<int>(baudrate))
           ? ESP_LOADER_SUCCESS
           : ESP_LOADER_ERROR_FAIL;
}

void EspFlasher::loader_port_debug_print(char const* str) { qDebug() << str; }

/// Log to trace.txt
void EspFlasher::transfer_debug_print(uint8_t const* data,
                                      uint16_t size,
                                      bool write) {
  if (_trace != "trace") return;

  static auto const log_path{
    QCoreApplication::applicationDirPath().toStdString() + "/../trace.log"};
  static auto fd{fopen(log_path.c_str(), "w")};
  static gsl::final_action close{[&fd] { fclose(fd); }};

  static bool write_prev{};

  if (write_prev != write) {
    write_prev = write;
    fprintf(fd, "\n--- %s ---\n", write ? "WRITE" : "READ");
  }

  for (auto i{0u}; i < size; i++) fprintf(fd, "%02x ", data[i]);
}
