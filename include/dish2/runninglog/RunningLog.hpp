#pragma once
#ifndef DISH2_RUNNINGLOG_RUNNINGLOG_HPP_INCLUDE
#define DISH2_RUNNINGLOG_RUNNINGLOG_HPP_INCLUDE

#include <deque>
#include <list>

#include "../config/cfg.hpp"

namespace dish2 {

template<typename Event>
class RunningLog {

  // (FRONT) newest events -> oldest events (BACK)
  std::deque<Event> events;

  // (FRONT) newest count -> oldest count ( BACK )
  std::deque< size_t > epoch_counts{ 0 };

public:

  using const_iterator = typename decltype(events)::const_iterator;
  using iterator = typename decltype(events)::iterator;

  void Record( const Event& event ){
    events.push_front( event );
    ++epoch_counts.front();
  }

  void Purge() {

    const size_t duration = dish2::cfg.RUNNING_LOG_DURATION();
    if ( epoch_counts.size() > duration ) {
      emp_assert( epoch_counts.size() - duration == 1 );
      events.resize( events.size() - epoch_counts.back() );
      epoch_counts.resize( duration );
    }

    epoch_counts.emplace_front();

  }

  const Event& GetMostRecent() const {
    emp_assert( events.size() );
    return events.front();
  }

  size_t GetSize() const { return events.size(); }
  size_t IsEmpty() const { return GetSize() == 0; }

  auto begin() { return std::begin( events ); }
  auto begin() const { return std::begin( events ); }
  auto cbegin() const { return std::cbegin( events ); }

  auto end() { return std::end( events ); }
  auto end() const { return std::end( events ); }
  auto cend() const { return std::cend( events ); }

};

} // namespace dish2



#endif // #ifndef DISH2_RUNNINGLOG_RUNNINGLOG_HPP_INCLUDE
