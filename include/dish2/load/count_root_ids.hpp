#pragma once
#ifndef DISH2_LOAD_COUNT_ROOT_IDS_HPP_INCLUDE
#define DISH2_LOAD_COUNT_ROOT_IDS_HPP_INCLUDE

#include "get_root_ids.hpp"

namespace dish2 {

size_t count_root_ids() { return dish2::get_root_ids().size(); }

} // namespace dish2

#endif // #ifndef DISH2_LOAD_COUNT_ROOT_IDS_HPP_INCLUDE
