#pragma once
#ifndef DISH2_RECORD_FINALIZE_ANIMATE_FRAMES_HPP_INCLUDE
#define DISH2_RECORD_FINALIZE_ANIMATE_FRAMES_HPP_INCLUDE

#include <cstdlib>
#include <iostream>
#include <string>

#include "../../../../third-party/conduit/include/uitsl/debug/err_verify.hpp"
#include "../../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "../../config/cfg.hpp"
#include "../../debug/log_msg.hpp"
#include "../../py/dump_animate_frames_script.hpp"

namespace dish2 {

void animate_frames() {
  dish2::log_msg( "animate_frames begin" );
  const std::string command = emp::to_string(
    "ls outframes/*.png | python3 ",
    dish2::dump_animate_frames_script(),
    " ",
    dish2::cfg.VIDEO_FPS(),
    " ",
    dish2::cfg.VIDEO_MAX_FRAMES()
  );
  uitsl::err_verify( std::system( command.c_str() ) );
}

} // namespace dish2

#endif // #ifndef DISH2_RECORD_FINALIZE_ANIMATE_FRAMES_HPP_INCLUDE
