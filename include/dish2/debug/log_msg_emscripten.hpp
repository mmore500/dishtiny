#pragma once
#ifndef DISH2_DEBUG_LOG_MSG_EMSCRIPTEN_HPP_INCLUDE
#define DISH2_DEBUG_LOG_MSG_EMSCRIPTEN_HPP_INCLUDE

#include <utility>

#include "../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "entry_types.hpp"
#include "log_event.hpp"

namespace dish2 {

template< typename... Args >
void log_msg_emscripten( Args&&... args ) {

  dish2::log_event( {
    dish2::info, emp::to_string( std::forward<Args>(args)... )
  } );

}

} // namespace dish2

#endif // #ifndef DISH2_DEBUG_LOG_MSG_EMSCRIPTEN_HPP_INCLUDE
