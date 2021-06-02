#pragma once
#ifndef DISH2_UTILITY_IS_NATIVE_HPP_INCLUDE
#define DISH2_UTILITY_IS_NATIVE_HPP_INCLUDE

#include "../../../third-party/conduit/include/uitsl/utility/UITSL_IF_WEB_ELSE.hpp"

namespace dish2 {

constexpr bool is_native() {

  return UITSL_IF_WEB_ELSE(false, true);

}

} // namespace dish2

#endif // #ifndef DISH2_UTILITY_IS_NATIVE_HPP_INCLUDE
