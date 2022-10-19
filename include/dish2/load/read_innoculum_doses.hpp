#pragma once
#ifndef DISH2_LOAD_READ_INNOCULUM_DOSES_HPP_INCLUDE
#define DISH2_LOAD_READ_INNOCULUM_DOSES_HPP_INCLUDE

#include <cstddef>

#include "../../../third-party/conduit/include/uitsl/math/math_utils.hpp"
#include "../../../third-party/conduit/include/uitsl/polyfill/filesystem.hpp"
#include "../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../third-party/Empirical/include/emp/tools/keyname_utils.hpp"

#include "../debug/log_msg.hpp"

#include "load_innoculum.hpp"

namespace dish2 {

emp::vector<size_t> read_innoculum_doses(
  const emp::vector<std::filesystem::path>& innoculum_paths
) {

  emp::vector<size_t> innoculum_doses;
  std::transform(
    std::begin( innoculum_paths ),
    std::end( innoculum_paths ),
    std::back_inserter( innoculum_doses ),
    [](const auto& path){
      const auto attrs = emp::keyname::unpack(path);
      if (attrs.count("dose")) {
        return uitsl::stoszt( attrs.at("dose") );
      } else return size_t{1}; // default dose is 1
    }
  );
  for (size_t i{}; i < innoculum_doses.size(); ++i) {
    dish2::log_msg( "bucket ", i, " has dose ", innoculum_doses[i] );
  }
  return innoculum_doses;

}

} // namespace dish2

#endif // #ifndef DISH2_LOAD_READ_INNOCULUM_DOSES_HPP_INCLUDE
