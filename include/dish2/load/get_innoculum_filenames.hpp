#pragma once
#ifndef DISH2_LOAD_GET_INNOCULUM_FILENAMES_HPP_INCLUDE
#define DISH2_LOAD_GET_INNOCULUM_FILENAMES_HPP_INCLUDE

#include <algorithm>
#include <string>

#include "../../../third-party/conduit/include/uitsl/polyfill/filesystem.hpp"
#include "../../../third-party/conduit/include/uitsl/utility/keyname_directory_transform.hpp"
#include "../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "get_innoculum_filename.hpp"
#include "get_root_ids.hpp"

namespace dish2 {

emp::vector<std::string> get_innoculum_filenames() {

  auto roots = dish2::get_root_ids();

  emp::vector<std::string> res;
  std::transform(
    std::begin( roots ), std::end( roots ),
    std::inserter( res, std::begin( res ) ),
    []( const size_t root_id ){
      return dish2::get_innoculum_filename( root_id );
    }
  );

  return res;

}

} // namespace dish2

#endif // #ifndef DISH2_LOAD_GET_INNOCULUM_FILENAMES_HPP_INCLUDE
