#pragma once
#ifndef UITSL_POLYFILL_IDENTITY_HPP_INCLUDE
#define UITSL_POLYFILL_IDENTITY_HPP_INCLUDE

#include <utility>

namespace std {

// TODO cpp20
// adpated from https://codereview.stackexchange.com/q/134627
static constexpr struct identity_t {
  template<typename T>
  constexpr decltype(auto) operator()(T&& t) const noexcept {
    return std::forward<T>(t);
  }
} identity{};

} // namespace std

#endif // #ifndef UITSL_POLYFILL_IDENTITY_HPP_INCLUDE
