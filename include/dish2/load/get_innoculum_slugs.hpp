#pragma once
#ifndef DISH2_LOAD_GET_INNOCULUM_SLUGS_HPP_INCLUDE
#define DISH2_LOAD_GET_INNOCULUM_SLUGS_HPP_INCLUDE

#include <algorithm>
#include <string>
#include <unordered_map>
#include <utility>

#include "../../../third-party/conduit/include/uitsl/polyfill/filesystem.hpp"
#include "../../../third-party/conduit/include/uitsl/utility/keyname_directory_transform.hpp"
#include "../../../third-party/Empirical/include/emp/base/always_assert.hpp"
#include "../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../third-party/Empirical/include/emp/tools/keyname_utils.hpp"
#include "../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "get_innoculum_slug.hpp"
#include "get_root_ids.hpp"

namespace dish2 {

std::unordered_map<size_t, std::string> get_innoculum_slugs() {

  auto roots = dish2::get_root_ids();

  std::unordered_map<size_t, std::string> res;
  std::transform(
    std::begin( roots ), std::end( roots ),
    std::inserter( res, std::begin( res ) ),
    []( const size_t root_id ){ return std::pair<size_t, std::string>{
      root_id, dish2::get_innoculum_slug( root_id )
    }; }
  );

  return res;

}

} // namespace dish2

#endif // #ifndef DISH2_LOAD_GET_INNOCULUM_SLUGS_HPP_INCLUDE
