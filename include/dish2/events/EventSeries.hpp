#pragma once
#ifndef DISH2_EVENTS_EVENTSERIES_HPP_INCLUDE
#define DISH2_EVENTS_EVENTSERIES_HPP_INCLUDE

namespace dish2 {

template<typename Event, size_t SeriesSize=0>
struct EventSeries {

  using event_t = Event;

  constexpr inline static size_t series_idx = SeriesSize;

  template< typename Cardinal, size_t SeriesIdx>
  static bool Test( const Cardinal& cardinal ) {
    return Event::template Test<Cardinal, SeriesIdx>( cardinal );
  }

  constexpr static size_t GetNumEvents() { return SeriesSize; }

};

} // namespace dish2

#endif // #ifndef DISH2_EVENTS_EVENTSERIES_HPP_INCLUDE
