#pragma once
#ifndef DISH2_UTILITY_ANIMATE_SCRIPT_HPP_INCLUDE
#define DISH2_UTILITY_ANIMATE_SCRIPT_HPP_INCLUDE

#include <filesystem>
#include <fstream>
#include <string>

#include "../../../third-party/conduit/include/uitsl/fetch/make_temp_filepath.hpp"
#include "../../../third-party/Empirical/include/emp/base/assert.hpp"

namespace dish2::py {

const std::string script_animate_frames{

#include "../../../script/animate_frames.py"

};

std::filesystem::path setup_script_animate() {

  const auto path = uitsl::make_temp_filepath();
  std::ofstream ofs(path);

  emp_assert( script_animate_frames.front() == '"' );
  emp_assert( script_animate_frames.back() == '"' );

  // strip leading and trailing aftifact "'s from included string
  ofs << script_animate_frames.substr(
    1, script_animate_frames.size() - 2
  );

  return path;

}

}

#endif // #ifndef DISH2_UTILITY_ANIMATE_SCRIPT_HPP_INCLUDE
