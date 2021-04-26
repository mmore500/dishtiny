#pragma once
#ifndef UITSL_DEBUG_AUDIT_COMPARE_HPP_INCLUDE
#define UITSL_DEBUG_AUDIT_COMPARE_HPP_INCLUDE

#include <limits>
#include <type_traits>

#include "audit_cast.hpp"

namespace uitsl {

template <typename I, typename J>
inline bool audit_greater(const I x, const J y) {

  if constexpr (
    std::numeric_limits<I>::is_signed != std::numeric_limits<J>::is_signed
  ) {
    return uitsl::audit_cast<typename std::make_signed<I>::type>( x )
      > uitsl::audit_cast<typename std::make_signed<J>::type>( y )
    ;
  } else return x > y;

}

template <typename I, typename J>
inline bool audit_less(const I x, const J y) {

  if constexpr (
    std::numeric_limits<I>::is_signed != std::numeric_limits<J>::is_signed
  ) {
    return uitsl::audit_cast<typename std::make_signed<I>::type>( x )
      < uitsl::audit_cast<typename std::make_signed<J>::type>( y )
    ;
  } else return x < y;

}

template <typename I, typename J>
inline bool audit_leq(const I x, const J y) {

  if constexpr (
    std::numeric_limits<I>::is_signed != std::numeric_limits<J>::is_signed
  ) {
    return uitsl::audit_cast<typename std::make_signed<I>::type>( x )
      <= uitsl::audit_cast<typename std::make_signed<J>::type>( y )
    ;
  } else return x <= y;

}

template <typename I, typename J>
inline bool audit_geq(const I x, const J y) {

  if constexpr (
    std::numeric_limits<I>::is_signed != std::numeric_limits<J>::is_signed
  ) {
    return uitsl::audit_cast<typename std::make_signed<I>::type>( x )
      >= uitsl::audit_cast<typename std::make_signed<J>::type>( y )
    ;
  } else return x >= y;

}

template <typename I, typename J>
inline bool audit_equal(const I x, const J y) {

  if constexpr (
    std::numeric_limits<I>::is_signed != std::numeric_limits<J>::is_signed
  ) {
    return uitsl::audit_cast<typename std::make_signed<I>::type>( x )
      == uitsl::audit_cast<typename std::make_signed<J>::type>( y )
    ;
  } else return x == y;

}

} // namespace uitsl

#endif // #ifndef UITSL_DEBUG_AUDIT_COMPARE_HPP_INCLUDE
