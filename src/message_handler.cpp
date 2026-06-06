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

/// Message handler
///
/// \file   message_handler.cpp
/// \author Vincent Hamp
/// \date   29/03/2024

#include "message_handler.hpp"
#include <functional>

namespace {

template<typename>
struct signature;

template<typename R, typename... Args>
struct signature<std::function<R(Args...)>> {
  using type = R(Args...);
  using return_type = R;
  using args = std::tuple<Args...>;
};

template<typename R, typename T, typename... Args>
struct signature<R (T::*)(Args...)> {
  using type = R(Args...);
  using return_type = R;
  using args = std::tuple<Args...>;
};

template<[[maybe_unused]] auto Unique = [] {}, typename F>
auto make_tramp(F&& f) {
  using Args =
    typename signature<decltype(std::function{std::declval<F>()})>::args;
  static auto _f{f};
  return []<size_t... Is>(std::index_sequence<Is...>) {
    return [](std::tuple_element_t<Is, Args>... args) {
      return std::invoke(_f, std::forward<decltype(args)>(args)...);
    };
  }(std::make_index_sequence<std::tuple_size_v<Args>>{});
}

} // namespace

/// Singleton pattern
MessageHandler* MessageHandler::get() {
  static MessageHandler message_handler;
  return &message_handler;
}

/// Install global Qt message handler
MessageHandler::MessageHandler() {
  qInstallMessageHandler(make_tramp(
    [this](QtMsgType type,
           QMessageLogContext const& context,
           QString const& msg) { emit messageHandler(type, context, msg); }));
}

/// Uninstall message handler
MessageHandler::~MessageHandler() { qInstallMessageHandler(nullptr); }
