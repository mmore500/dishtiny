#pragma once
#ifndef DISH2_LOAD_GET_INNOCULUM_FILENAME_HPP_INCLUDE
#define DISH2_LOAD_GET_INNOCULUM_FILENAME_HPP_INCLUDE

#include <algorithm>
#include <string>

#include "../../../third-party/conduit/include/uitsl/polyfill/filesystem.hpp"
#include "../../../third-party/Empirical/include/emp/base/always_assert.hpp"
#include "../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../third-party/Empirical/include/emp/tools/keyname_utils.hpp"
#include "../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

namespace dish2 {

std::string get_innoculum_filename( const size_t root_id ) {

  emp::vector<std::filesystem::path> innoculum_paths;
  std::copy_if(
    std::filesystem::directory_iterator( "." ),
    std::filesystem::directory_iterator(),
    std::back_inserter( innoculum_paths ),
    [&]( const auto& entry ){
      const auto attrs = emp::keyname::unpack( entry.path() );
      return attrs.count("a") && attrs.at("a") == "genome"
        && attrs.count("root_id")
          && attrs.at("root_id") == emp::to_string( root_id )
        && attrs.count("ext") && attrs.at("ext") == ".json";
    }
  );

  emp_always_assert( innoculum_paths.size() == 1, innoculum_paths.size() );

  return innoculum_paths.front() ;

}

} // namespace dish2

#endif // #ifndef DISH2_LOAD_GET_INNOCULUM_FILENAME_HPP_INCLUDE
