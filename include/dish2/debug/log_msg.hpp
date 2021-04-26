#pragma once
#ifndef DISH2_DEBUG_LOG_MSG_HPP_INCLUDE
#define DISH2_DEBUG_LOG_MSG_HPP_INCLUDE

#include <utility>

#include "log_msg_emscripten.hpp"
#include "log_msg_native.hpp"

namespace dish2 {

template< typename... Args >
void log_msg( Args&&... args ) {

#ifdef __EMSCRIPTEN__
  dish2::log_msg_emscripten( std::forward<Args>(args)... );
#else // #ifdef __EMSCRIPTEN__
  dish2::log_msg_native( std::forward<Args>(args)... );
#endif // #ifdef __EMSCRIPTEN__

}

} // namespace dish2

#endif // #ifndef DISH2_DEBUG_LOG_MSG_HPP_INCLUDE
