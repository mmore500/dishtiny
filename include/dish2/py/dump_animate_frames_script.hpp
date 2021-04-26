#pragma once
#ifndef DISH2_PY_DUMP_ANIMATE_FRAMES_SCRIPT_HPP_INCLUDE
#define DISH2_PY_DUMP_ANIMATE_FRAMES_SCRIPT_HPP_INCLUDE

#include <fstream>
#include <string>

#include "../../../third-party/conduit/include/uitsl/fetch/make_temp_filepath.hpp"
#include "../../../third-party/Empirical/include/emp/base/assert.hpp"

#include "strip_pyinclude_artifacts.hpp"

namespace dish2 {


std::filesystem::path dump_animate_frames_script() {

  const std::string script_text = dish2::strip_pyinclude_artifacts(
    #include "../../../script/animate_frames.py"
  );

  const auto path = uitsl::make_temp_filepath();
  std::ofstream ofs(path);
  ofs << script_text;

  return path;

}

}

#endif // #ifndef DISH2_PY_DUMP_ANIMATE_FRAMES_SCRIPT_HPP_INCLUDE
