#pragma once
#ifndef DISH2_RUNNINGLOG_DEATHEVENT_HPP_INCLUDE
#define DISH2_RUNNINGLOG_DEATHEVENT_HPP_INCLUDE

#include <deque>

namespace dish2 {

template< typename Spec >
struct DeathEvent {

  dish2::CauseOfDeath cause_of_death;
  emp::array<size_t, Spec::NLEV> peripherality_deceased;

};

} // namespace dish2




#endif // #ifndef DISH2_RUNNINGLOG_DEATHEVENT_HPP_INCLUDE
