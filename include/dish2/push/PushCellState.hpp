#pragma once
#ifndef DISH2_PUSH_PUSHCELLSTATE_HPP_INCLUDE
#define DISH2_PUSH_PUSHCELLSTATE_HPP_INCLUDE

#include <tuple>

#include "DistanceToGraphCenterCellState.hpp"

namespace dish2 {

using PushCellState = std::tuple<
  dish2::DistanceToGraphCenterCellState
>;

} // namespace dish2

#endif // #ifndef DISH2_PUSH_PUSHCELLSTATE_HPP_INCLUDE
