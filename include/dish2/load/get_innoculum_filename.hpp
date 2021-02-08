#pragma once
#ifndef DISH2_LOAD_GET_INNOCULUM_FILENAME_HPP_INCLUDE
#define DISH2_LOAD_GET_INNOCULUM_FILENAME_HPP_INCLUDE

#include <algorithm>
#include <iterator>
#include <string>

#include "../../../third-party/conduit/include/uitsl/polyfill/filesystem.hpp"
#include "../../../third-party/Empirical/include/emp/base/always_assert.hpp"
#include "../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../third-party/Empirical/include/emp/tools/keyname_utils.hpp"
#include "../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "../load/count_root_ids.hpp"
#include "../load/get_root_ids.hpp"

namespace dish2 {

std::string get_innoculum_filename( const size_t root_id ) {

  using dir_it_t = std::filesystem::directory_iterator;

  emp::vector<std::filesystem::path> innoculum_paths;
  std::copy_if(
    dir_it_t( "." ), dir_it_t(),
    std::back_inserter( innoculum_paths ),
    [&]( const auto& entry ){
      const auto attrs = emp::keyname::unpack( entry.path() );
      return attrs.count("a") && attrs.at("a") == "genome"
        && attrs.count("root_id")
          && attrs.at("root_id") == emp::to_string( root_id )
        && attrs.count("ext") && (
          attrs.at("ext") == ".json"
          || attrs.at("ext") == ".json.gz"
          || attrs.at("ext") == ".bin"
          || attrs.at("ext") == ".bin.gz"
        );
    }
  );

  emp_always_assert(
    innoculum_paths.size() == 1,
    innoculum_paths.size(),
    root_id,
    std::distance( dir_it_t( "." ), dir_it_t() ),
    dish2::count_root_ids(),
    emp::to_string( dish2::get_root_ids() )
  );

  return innoculum_paths.front();

}

} // namespace dish2

#endif // #ifndef DISH2_LOAD_GET_INNOCULUM_FILENAME_HPP_INCLUDE
