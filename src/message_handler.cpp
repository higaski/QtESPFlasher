// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

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
