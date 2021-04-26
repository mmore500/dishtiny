#pragma once
#ifndef UITSL_DEBUG_AUDIT_CAST_HPP_INCLUDE
#define UITSL_DEBUG_AUDIT_CAST_HPP_INCLUDE

#include "safe_cast.hpp"

namespace uitsl {

template<typename Dst, typename Src>
inline Dst audit_cast(const Src value) {
  #ifndef NDEBUG
    return uitsl::safe_cast<Dst, Src>( value );
  #else
    return static_cast<Dst>( value );
  #endif
}

} // namespace uitsl

#endif // #ifndef UITSL_DEBUG_AUDIT_CAST_HPP_INCLUDE
