// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// GUI options
///
/// \file   com_box.hpp
/// \author Vincent Hamp
/// \date   29/03/2024

#pragma once

#include <QCheckBox>
#include <QComboBox>
#include <QGroupBox>
#include <QPushButton>
#include <QThread>
#include <esp_flasher/esp_flasher.hpp>

/// Bottom part GUI widget which displays serial port options
///
/// ComBox is a QGroupBox widget which displays a couple of dropdown menus to
/// choose various serial port options. Apart from that, there is a start/stop
/// button to start the writing process.
class ComBox : public QGroupBox {
  Q_OBJECT

public:
  ComBox(QWidget* parent = nullptr);

public slots:
  void binaries(QVector<Bin> bins);

private slots:
  void startStopButtonClicked(bool start);

private:
  QComboBox* _chip_combobox{new QComboBox};
  QComboBox* _port_combobox{new QComboBox};
  QComboBox* _baud_combobox{new QComboBox};
  QComboBox* _before_combobox{new QComboBox};
  QComboBox* _after_combobox{new QComboBox};
  QCheckBox* _no_stub_checkbox{new QCheckBox};
  QCheckBox* _trace_checkbox{new QCheckBox};
  QPushButton* _start_stop_button{new QPushButton};
  QVector<Bin> _bins{};
  QThread* _thread{};
  EspFlasher* _esp_flasher{};
};
