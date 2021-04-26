#pragma once
#ifndef UITSL_POLYFILL_IS_CONSTANT_EVALUATED_HPP_INCLUDE
#define UITSL_POLYFILL_IS_CONSTANT_EVALUATED_HPP_INCLUDE

namespace std {

constexpr bool is_constant_evaluated(const int impl=0) noexcept {

  // adapted from https://stackoverflow.com/a/62610143
  // exclude clang versions with compiler bug https://reviews.llvm.org/D35190
  #if defined(__clang__) && __clang_major__>=9 \
    || defined(__GNUC__) && !defined(__clang__)
    // if impl is known at compile time,
    // we are in a constevaluated context
    return __builtin_constant_p( impl );
  #else
    // otherwise, default to a constexpr-friendly implementation
    return true;
  #endif

}

} // namespace std

#endif // #ifndef UITSL_POLYFILL_IS_CONSTANT_EVALUATED_HPP_INCLUDE
