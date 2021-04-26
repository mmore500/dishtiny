#pragma once
#ifndef DISH2_PY_STRIP_PYINCLUDE_ARTIFACTS_HPP_INCLUDE
#define DISH2_PY_STRIP_PYINCLUDE_ARTIFACTS_HPP_INCLUDE

#include <fstream>
#include <string>

#include "../../../third-party/conduit/include/uitsl/fetch/make_temp_filepath.hpp"
#include "../../../third-party/Empirical/include/emp/base/assert.hpp"

namespace dish2 {

std::string strip_pyinclude_artifacts( const std::string& raw_include ) {

  emp_assert( raw_include.size() >= 2, raw_include.size() );
  emp_assert( raw_include.front() == '"', raw_include.front() );
  emp_assert( raw_include.back() == '"', raw_include.back() );

  // strip leading and trailing aftifact "'s from included string
  return raw_include.substr( 1, raw_include.size() - 2 );

}

} // namespace dish2

#endif // #ifndef DISH2_PY_STRIP_PYINCLUDE_ARTIFACTS_HPP_INCLUDE
