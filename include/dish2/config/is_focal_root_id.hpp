#pragma once
#ifndef DISH2_CONFIG_IS_FOCAL_ROOT_ID_HPP_INCLUDE
#define DISH2_CONFIG_IS_FOCAL_ROOT_ID_HPP_INCLUDE

#include <algorithm>

#include "../genome/RootID.hpp"

#include "cfg.hpp"

namespace dish2 {

bool is_focal_root_id(const dish2::RootID root_id) {

  return std::find(
    std::begin(cfg.COALESCENCE_EXCLUDE_ROOT_IDS()),
    std::end(cfg.COALESCENCE_EXCLUDE_ROOT_IDS()),
    root_id.GetID()
  ) == std::end(cfg.COALESCENCE_EXCLUDE_ROOT_IDS());

}

} // namespace dish2

#endif // #ifndef DISH2_CONFIG_IS_FOCAL_ROOT_ID_HPP_INCLUDE
