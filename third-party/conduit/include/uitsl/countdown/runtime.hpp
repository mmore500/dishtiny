#pragma once
#ifndef UITSL_COUNTDOWN_RUNTIME_HPP_INCLUDE
#define UITSL_COUNTDOWN_RUNTIME_HPP_INCLUDE

#include <chrono>

#include "Timer.hpp"

namespace uitsl {

template<
  typename Duration_T=std::chrono::duration<double, std::ratio<1>>,
  typename Clock_T=std::chrono::steady_clock
>
uitsl::Timer<Duration_T, Clock_T> runtime;


} // namespace uitsl

#endif // #ifndef UITSL_COUNTDOWN_RUNTIME_HPP_INCLUDE
