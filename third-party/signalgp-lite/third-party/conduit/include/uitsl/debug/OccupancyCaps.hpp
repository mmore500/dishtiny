#pragma once
#ifndef UITSL_DEBUG_OCCUPANCYCAPS_HPP_INCLUDE
#define UITSL_DEBUG_OCCUPANCYCAPS_HPP_INCLUDE

#include <limits>
#include <stddef.h>
#include <string>
#include <tuple>
#include <unordered_map>

#include "../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "OccupancyCap.hpp"

namespace uitsl {

class OccupancyCaps {

  std::unordered_map<int, OccupancyCap> lookup;
  mutable std::mutex mutex;

public:

  OccupancyCaps() = default;

  OccupancyCaps(const OccupancyCaps&) { ; }

  OccupancyCaps& operator=(const OccupancyCaps&) { return *this; }

  OccupancyCap& Get(
    const int& line,
    const size_t maximum_occupancy=std::numeric_limits<size_t>::max()
  ) {

    const std::lock_guard guard{mutex};

    if (!lookup.count(line)) {
      auto res = lookup.emplace(
        std::piecewise_construct,
        std::tuple{line},
        std::tuple{maximum_occupancy}
      );
      return res.first->second;
    } else {
      auto & res = lookup.at(line);
      emp_assert(
        res.GetMaximumOccupancy() == maximum_occupancy,
        [](){ error_message_mutex.lock(); return "locked"; }(),
        maximum_occupancy,
        res.GetMaximumOccupancy()
      );
      return res;
    }

  }

};

} // namespace uitsl

#endif // #ifndef UITSL_DEBUG_OCCUPANCYCAPS_HPP_INCLUDE
