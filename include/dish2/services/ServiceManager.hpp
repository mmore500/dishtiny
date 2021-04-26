#pragma once
#ifndef DISH2_SERVICES_SERVICEMANAGER_HPP_INCLUDE
#define DISH2_SERVICES_SERVICEMANAGER_HPP_INCLUDE

namespace dish2 {

// base case
template<typename... Services> struct ServiceManager {

  template<typename Cell>
  static void Run( Cell& cell, const size_t, const bool alive ) {}

};

// adapted from https://stackoverflow.com/a/35284581
template<typename FirstService, typename... SubsequentServices>
struct ServiceManager<FirstService, SubsequentServices...> {

  template<typename Cell>
  static void Run( Cell& cell, const size_t update, const bool alive ) {

    using subsequent_services_t = dish2::ServiceManager<SubsequentServices...>;

    if ( FirstService::ShouldRun( update, alive ) ) {
      FirstService::DoService( cell );
    }

    if constexpr ( sizeof...(SubsequentServices) > 0 ) {
      subsequent_services_t::template Run<Cell>( cell, update, alive );
    }

  }

};

} // namespace dish2

#endif // #ifndef DISH2_SERVICES_SERVICEMANAGER_HPP_INCLUDE
