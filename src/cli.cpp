// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// CLI
///
/// \file   cli.cpp
/// \author Vincent Hamp
/// \date   29/03/2024

#include "cli.hpp"
#include <QCommandLineParser>
#include <QFileInfo>
#include <esp_flasher/esp_flasher.hpp>
#include <thread>

namespace {

/// Join range with separator
///
/// \tparam R         std::ranges::input_range
/// \param  r         Range
/// \param  separator Separator
/// \return Joined range
template<std::ranges::input_range R>
auto join_with_separator(R&& r, char separator) {
  auto retval{std::accumulate(cbegin(r),
                              cend(r),
                              QString{},
                              [separator](auto const& lhs, auto const& rhs) {
                                return lhs + separator + rhs;
                              })};
  retval.remove(0, 2);
  return retval;
}

/// Print all available ports
///
/// \retval 0
int print_available_ports() {
  for (auto const& port_info : available_ports())
    qInfo() << "\n"
            << "Port:" << port_info.portName() << "\n"
            << "Location:" << port_info.systemLocation() << "\n"
            << "Description:" << port_info.description() << "\n"
            << "Manufacturer:" << port_info.manufacturer() << "\n"
            << "Serial number:" << port_info.serialNumber() << "\n"
            << "Vendor Identifier:"
            << (port_info.hasVendorIdentifier()
                  ? QByteArray::number(port_info.vendorIdentifier(), 16)
                  : QByteArray())
            << "\n"
            << "Product Identifier:"
            << (port_info.hasProductIdentifier()
                  ? QByteArray::number(port_info.productIdentifier(), 16)
                  : QByteArray());
  return 0;
}

/// Execute write_flash command
///
/// \param  parser  Command line parser
/// \retval 0       Success
/// \retval -1      Error
int write_flash(QCommandLineParser const& parser) {
  auto pos_args{parser.positionalArguments()};
  pos_args.pop_front();

  if (pos_args.size() % 2) {
    qCritical() << "write_flash: error: argument <address> <filename>: Must be "
                   "pairs of an address and the binary filename to write there";
    return -1;
  }

  // Defaults
  QString const chip{parser.isSet("chip") ? parser.value("chip") : "auto"};
  QString const port{parser.isSet("port") ? parser.value("port") : "auto"};
  QString const baud{parser.isSet("baud") ? parser.value("baud") : "auto"};
  QString const before{parser.isSet("before") ? parser.value("before")
                                              : "default_reset"};
  QString const after{parser.isSet("after") ? parser.value("after")
                                            : "hard_reset"};
  QString const no_stub{parser.isSet("no-stub") ? "no-stub" : ""};
  QString const trace{parser.isSet("trace") ? "trace" : ""};

  // Gather binaries
  QVector<Bin> bins;
  for (int i{}; i < pos_args.size(); i += 2) {
    bool ok{};
    auto const offset{pos_args[i].toUInt(&ok, 0)};
    if (!ok) {
      qCritical()
        << "write_flash: error: argument <address> <filename>: Address"
        << pos_args[i] << "must be a number";
      return -1;
    }

    QFile file{pos_args[i + 1]};
    if (!file.open(QIODeviceBase::ReadOnly)) {
      qCritical() << "write_flash: error: argument <address> <filename>: No "
                     "such file or directory:"
                  << pos_args[i + 1];
      return -1;
    }
    bins.push_back({.offset = offset, .bytes = file.readAll()});
  }

  EspFlasher esp_flasher{chip, port, baud, before, after, no_stub, trace, bins};
  return esp_flasher.flash();
}

/// Execute erase_flash command
///
/// \param  parser  Command line parser
/// \retval 0       Success
/// \retval -1      Error
int erase_flash(QCommandLineParser const& parser) {
  // Defaults
  QString const chip{parser.isSet("chip") ? parser.value("chip") : "auto"};
  QString const port{parser.isSet("port") ? parser.value("port") : "auto"};
  QString const baud{parser.isSet("baud") ? parser.value("baud") : "auto"};
  QString const before{parser.isSet("before") ? parser.value("before")
                                              : "default_reset"};
  QString const after{parser.isSet("after") ? parser.value("after")
                                            : "hard_reset"};
  QString const no_stub{parser.isSet("no-stub") ? "no-stub" : ""};
  QString const trace{parser.isSet("trace") ? "trace" : ""};

  EspFlasher esp_flasher{chip, port, baud, before, after, no_stub, trace};
  return esp_flasher.erase();
}

} // namespace

/// Run CLI
///
/// \param  app Application instance
/// \return ​0​ if the CLI succeeds, nonzero value otherwise
int cli(QApplication const& app) {
  QCommandLineParser parser;
  parser.addHelpOption();
  parser.addVersionOption();

  // Add options
  parser.addOptions({
    {{"c", "chip"}, "Target chip type", join_with_separator(chips, ',')},
    {{"p", "port"}, "Serial port device", "PORT"},
    {{"b", "baud"}, "Serial port baud rate", "BAUD"},
    {"before",
     "What to do before connecting to the chip",
     join_with_separator(befores, ',')},
    {{"a", "after"},
     "What to do after QtESPFlasher is finished",
     join_with_separator(afters, ',')},
    {"no-stub",
     "Disable launching the flasher stub, only talk to ROM bootloader"},
    {{"t", "trace"}, "Enable trace-level output of QtESPFlasher interactions"},
    {{"l", "list"},
     "List all available serial interfaces"}, // From STM32CubeProgrammer
  });

  // Add commands
  parser.addPositionalArgument(
    "write_flash",
    "Write a binary blob to flash\nwrite_flash "
    "<address> <filename> [<address> <filename> ...]");
  parser.addPositionalArgument("erase_flash",
                               "Perform Chip Erase on SPI flash");

  // Parse arguments
  parser.process(app);

  // If list is set, do nothing else
  if (parser.isSet("list")) return print_available_ports();

  // Execute commands
  if (auto pos_args{parser.positionalArguments()}; !pos_args.size()) return 0;
  else if (pos_args.first() == "write_flash") return write_flash(parser);
  else if (pos_args.first() == "erase_flash") return erase_flash(parser);
  else return -1;
}
