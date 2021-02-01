#pragma once
#ifndef DISH2_LOAD_GET_ROOT_IDS_HPP_INCLUDE
#define DISH2_LOAD_GET_ROOT_IDS_HPP_INCLUDE

#include "../../../third-party/conduit/include/uitsl/math/math_utils.hpp"
#include "../../../third-party/conduit/include/uitsl/utility/keyname_directory_transform.hpp"
#include "../../../third-party/Empirical/include/emp/base/vector.hpp"

namespace dish2 {

emp::vector<size_t> get_root_ids() {

  return uitsl::keyname_directory_transform(
    "root_id",
    { {"a", "genome"}, {"ext", ".json"} },
    ".",
    uitsl::stoszt
  );

}

} // namespace dish2

#endif // #ifndef DISH2_LOAD_GET_ROOT_IDS_HPP_INCLUDE
