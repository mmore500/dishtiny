#pragma once
#ifndef UITSL_DEBUG_OCCUPANCYCAP_HPP_INCLUDE
#define UITSL_DEBUG_OCCUPANCYCAP_HPP_INCLUDE

#include <limits>
#include <mutex>
#include <stddef.h>
#include <unordered_set>

#include "../parallel/thread_utils.hpp"
#include "../utility/print_utils.hpp"

namespace uitsl {

class OccupancyCap {

  mutable std::mutex mutex;

  std::unordered_set<thread_id_t> occupants;

  const size_t maximum_occupancy;

public:

  OccupancyCap(
    const size_t maximum_occupancy_=std::numeric_limits<size_t>::max()
  ) : maximum_occupancy(maximum_occupancy_)
  { ; }

  void Enter() {

    const std::lock_guard guard{mutex};

    const thread_id_t id = get_thread_id();

    emp_assert(
      occupants.count(id) == 0,
      [](){ error_message_mutex.lock(); return "locked"; }(),
      occupants.count(id)
    );

    occupants.insert(id);

    emp_assert(
      occupants.size() <= maximum_occupancy,
      [](){ error_message_mutex.lock(); return "locked"; }(),
      occupants.size(),
      maximum_occupancy,
      to_string(occupants)
    );

  }

  size_t GetMaximumOccupancy() const { return maximum_occupancy; }


  void Exit() {

    const std::lock_guard guard{mutex};

    const thread_id_t id = get_thread_id();

    emp_assert(
      occupants.count(id),
      [](){ error_message_mutex.lock(); return "locked"; }()
    );

    occupants.erase(id);

  }

};

} // namespace uitsl

#endif // #ifndef UITSL_DEBUG_OCCUPANCYCAP_HPP_INCLUDE
