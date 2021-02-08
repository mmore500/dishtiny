#pragma once
#ifndef DISH2_LOAD_GET_ROOT_IDS_HPP_INCLUDE
#define DISH2_LOAD_GET_ROOT_IDS_HPP_INCLUDE

#include <set>
#include <string>

#include "../../../third-party/conduit/include/uitsl/math/math_utils.hpp"
#include "../../../third-party/conduit/include/uitsl/utility/keyname_directory_transform.hpp"
#include "../../../third-party/Empirical/include/emp/base/always_assert.hpp"
#include "../../../third-party/Empirical/include/emp/base/array.hpp"
#include "../../../third-party/Empirical/include/emp/base/vector.hpp"

namespace dish2 {

emp::vector<size_t> get_root_ids() {

  emp::vector<size_t> res;

  const emp::array<std::string, 4> exts{".json", ".json.gz", ".bin", ".bin.gz"};
  for (const auto& ext : exts ) {
    const auto candidates = uitsl::keyname_directory_transform(
      "root_id",
      { {"a", "genome"}, {"ext", ext} },
      ".",
      uitsl::stoszt
    );
    res.insert(
      std::end( res ), std::begin( candidates ), std::end( candidates )
    );
  }

  // ensure each candidate has a unique root id
  emp_always_assert(
    std::set<size_t>( std::begin(res), std::end(res) ).size() == res.size()
  );

  return res;

}

} // namespace dish2

#endif // #ifndef DISH2_LOAD_GET_ROOT_IDS_HPP_INCLUDE
