#pragma once
#ifndef DISH2_ENUM_CAUSEOFDEATH_HPP_INCLUDE
#define DISH2_ENUM_CAUSEOFDEATH_HPP_INCLUDE

#include <iostream>

#include "../../../third-party/magic_enum/include/magic_enum.hpp"

namespace dish2 {

enum class CauseOfDeath {
  unknown,
  age,
  apoptosis,
  elimination,
  fragmentation,
  quorum,
};

inline std::ostream& operator<<(
  std::ostream& os, const dish2::CauseOfDeath& val
) {
  return magic_enum::ostream_operators::operator<<(os, val);
}

} // namespace dish2

#endif // #ifndef DISH2_ENUM_CAUSEOFDEATH_HPP_INCLUDE
