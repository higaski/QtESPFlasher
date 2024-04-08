// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// GUI options
///
/// \file   com_box.cpp
/// \author Vincent Hamp
/// \date   29/03/2024

#include "com_box.hpp"
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSerialPortInfo>
#include "options.hpp"
#include "update_ports_event_filter.hpp"

/// Create layout of various dropdown menus and a start/stop button
ComBox::ComBox(QWidget* parent) : QGroupBox{parent} {
  // Start/stop button
  QPixmap pixmap_off{":/dark/play.svg"};
  QPixmap pixmap_on{":/dark/stop.svg"};
  QIcon ico;
  ico.addPixmap(pixmap_off, QIcon::Normal, QIcon::Off);
  ico.addPixmap(pixmap_on, QIcon::Normal, QIcon::On);
  _start_stop_button->setIcon(ico);
  _start_stop_button->setText("Start");
  _start_stop_button->setCheckable(true);

  // Add before/after options
  for (auto const& before : befores) _before_combobox->addItem(before);
  _before_combobox->setToolTip("What to do before connecting to the chip");
  for (auto const& after : afters) _after_combobox->addItem(after);
  _after_combobox->setToolTip("What to do after QtESPFlasher is finished");

  // Add --no-stub and --trace option
  auto row{new QHBoxLayout};
  row->addWidget(new QLabel{"--no-stub"});
  row->addWidget(_no_stub_checkbox);
  _no_stub_checkbox->setToolTip(
    "Disable launching the flasher stub, only talk to ROM bootloader");
  row->addWidget(new QLabel{"--trace"});
  row->addWidget(_trace_checkbox);
  _trace_checkbox->setToolTip(
    "Enable trace-level output of QtESPFlasher interactions");

  // Chips dropdown
  for (auto const& chip : chips) _chip_combobox->addItem(chip);
  _chip_combobox->setCurrentIndex(_chip_combobox->findText("auto"));
  _chip_combobox->setToolTip("Target chip type");

  // Port dropdown
  _port_combobox->addItem("auto");
  // Install an event filter which updates list of serial ports every time the
  // dropdown is selected
  _port_combobox->installEventFilter(new UpdatePortsEventFilter{this});
  _port_combobox->setToolTip("Serial port device");

  // Baud dropdown
  _baud_combobox->addItem("auto");
  for (auto const& baud_rate : QSerialPortInfo::standardBaudRates())
    if (baud_rate <= 115200)
      _baud_combobox->addItem(QString::number(baud_rate));
  _baud_combobox->setToolTip(
    "Serial port baud rate used when flashing/reading");

  // Layout
  auto layout{new QGridLayout};
  // Workaround: top margin must be zero for the layout to be vertically
  // centered
  layout->setContentsMargins(11, 0, 11, 11);
  layout->addWidget(_start_stop_button, 0, 0);
  layout->addWidget(new QLabel{"Before"}, 0, 1, Qt::AlignRight);
  layout->addWidget(new QLabel{"After"}, 1, 1, Qt::AlignRight);
  layout->addWidget(_before_combobox, 0, 2);
  layout->addWidget(_after_combobox, 1, 2);
  layout->addWidget(new QLabel{"Options"}, 2, 1, Qt::AlignRight);
  layout->addLayout(row, 2, 2, Qt ::AlignHCenter);
  layout->addWidget(new QLabel{"Target"}, 0, 3, Qt::AlignRight);
  layout->addWidget(_chip_combobox, 0, 4);
  layout->addWidget(new QLabel{"Com"}, 1, 3, Qt::AlignRight);
  layout->addWidget(_port_combobox, 1, 4);
  layout->addWidget(new QLabel{"Baud"}, 2, 3, Qt::AlignRight);
  layout->addWidget(_baud_combobox, 2, 4);
  setLayout(layout);

  connect(_start_stop_button,
          &QPushButton::clicked,
          this,
          &ComBox::startStopButtonClicked);
}

/// Set binaries slot
///
/// \param  bins  Binaries
void ComBox::binaries(QVector<Bin> bins) { _bins = bins; }

/// Start/stop button slot
///
/// \param  start
void ComBox::startStopButtonClicked(bool start) {
  // Start thread
  if (start) {
    _start_stop_button->setText("Stop");

    QString const chip{_chip_combobox->currentText()};
    QString const port{_port_combobox->currentText()};
    QString const baud{_baud_combobox->currentText()};
    QString const before{_before_combobox->currentText()};
    QString const after{_after_combobox->currentText()};
    QString const no_stub{
      _no_stub_checkbox->checkState() == Qt::Checked ? "no-stub" : ""};
    QString const trace{_trace_checkbox->checkState() == Qt::Checked ? "trace"
                                                                     : ""};

    _thread = new QThread;
    _esp_flasher =
      new EspFlasher{chip, port, baud, before, after, no_stub, trace, _bins};
    _esp_flasher->moveToThread(_thread);

    // Thread starts loader
    connect(_thread,
            &QThread::started,
            _esp_flasher,
            qOverload<>(&EspFlasher::flash));

    // When loader finishes, quit thread and delete loader
    connect(_esp_flasher, &EspFlasher::finished, _thread, &QThread::quit);
    connect(_esp_flasher,
            &EspFlasher::finished,
            _esp_flasher,
            &EspFlasher::deleteLater);

    // When thread finished, delete thread
    connect(_thread, &QThread::finished, _thread, &QThread::deleteLater);

    // Once thread is destroyed, reset button
    connect(_thread, &QThread::destroyed, [this] {
      _start_stop_button->setChecked(false);
      _start_stop_button->setText("Start");
    });

    _thread->start();
  }
  // Stop running thread
  else if (_thread->isRunning())
    _thread->requestInterruption();
}
