#pragma once
#ifndef DISH2_EVENTS_EVENTMANAGER_HPP_INCLUDE
#define DISH2_EVENTS_EVENTMANAGER_HPP_INCLUDE

#include "EventSeries.hpp"

namespace dish2 {

// base case
template<typename... EventSeries>
struct EventManager {

  template<typename Cardinal, typename EventTags, size_t EventIdx>
  static void Dispatch(const Cardinal&, const EventTags&){}

  constexpr static size_t GetNumEvents() { return 0; }

};

// adapted from https://stackoverflow.com/a/35284581
template<typename FirstEventSeries, typename... SubsequentEventSeries>
struct EventManager<FirstEventSeries, SubsequentEventSeries...> {

  constexpr inline static size_t series_idx = FirstEventSeries::series_idx;

  template<typename Cardinal, typename EventTags, size_t EventIdx=0>
  static void Dispatch(Cardinal& cardinal, const EventTags& event_tags) {

    // dispatch event
    if ( FirstEventSeries::template Test<Cardinal, series_idx>( cardinal ) ) {
      cardinal.DispatchEvent( event_tags.tags[ EventIdx ] );
    }

    // recurse, either to next event in series or to next event series
    if constexpr ( series_idx > 0 ) {
      using next_in_series_t = dish2::EventSeries<
        typename FirstEventSeries::event_t, series_idx - 1
      >;
      using next_event_manager = dish2::EventManager<
        next_in_series_t,
        SubsequentEventSeries...
      >;
      next_event_manager::template Dispatch<
        Cardinal, EventTags, EventIdx + 1
      >(
        cardinal, event_tags
      );
    } else if constexpr ( sizeof...(SubsequentEventSeries) > 0 ) {
      using subsequent_series_t = dish2::EventManager<SubsequentEventSeries...>;
      subsequent_series_t::template Dispatch<
        Cardinal, EventTags, EventIdx + 1
      >(
        cardinal, event_tags
      );
    }

  }

  constexpr static size_t GetNumEvents() {
    // recurse, either to next event in series or to next event series
    if constexpr ( series_idx > 0 ) {
      using next_in_series_t = dish2::EventSeries<
        typename FirstEventSeries::event_t, series_idx - 1
      >;
      using next_event_manager = dish2::EventManager<
        next_in_series_t,
        SubsequentEventSeries...
      >;
    return next_event_manager::GetNumEvents() + 1;
    } else if constexpr ( sizeof...(SubsequentEventSeries) > 0 ) {
      using subsequent_series_t = dish2::EventManager<SubsequentEventSeries...>;
      return subsequent_series_t::GetNumEvents() + 1;
    } else return 1;
  }

};

} // namespace dish2

#endif // #ifndef DISH2_EVENTS_EVENTMANAGER_HPP_INCLUDE
