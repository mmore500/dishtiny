#pragma once
#ifndef DISH2_LOAD_COUNT_FOCAL_ROOT_IDS_HPP_INCLUDE
#define DISH2_LOAD_COUNT_FOCAL_ROOT_IDS_HPP_INCLUDE

#include <algorithm>

#include "../config/is_focal_root_id.hpp"

#include "get_root_ids.hpp"

namespace dish2 {

size_t count_focal_root_ids() {
  const auto root_ids = dish2::get_root_ids();
  return std::count_if(
    std::begin(root_ids),
    std::end(root_ids),
    dish2::is_focal_root_id
  );
}

} // namespace dish2

#endif // #ifndef DISH2_LOAD_COUNT_FOCAL_ROOT_IDS_HPP_INCLUDE
