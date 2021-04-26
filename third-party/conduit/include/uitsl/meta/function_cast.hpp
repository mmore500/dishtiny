#pragma once
#ifndef UITSL_META_FUNCTION_CAST_HPP_INCLUDE
#define UITSL_META_FUNCTION_CAST_HPP_INCLUDE

#include <functional>

namespace uitsl {

namespace internal {

template<typename T>
struct memfun_type {
  using type = void;
};

template<typename Ret, typename Class, typename... Args>
struct memfun_type<Ret(Class::*)(Args...) const> {
  using type = std::function<Ret(Args...)>;
};

} // namespace internal

// adapted from https://stackoverflow.com/a/24068396
template<typename F>
typename internal::memfun_type<decltype(&F::operator())>::type
function_cast(F const &func) {
  return func;
}

} // namespace uitsl

#endif // #ifndef UITSL_META_FUNCTION_CAST_HPP_INCLUDE
