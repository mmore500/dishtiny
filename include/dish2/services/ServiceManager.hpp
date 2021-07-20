#pragma once
#ifndef DISH2_SERVICES_SERVICEMANAGER_HPP_INCLUDE
#define DISH2_SERVICES_SERVICEMANAGER_HPP_INCLUDE

namespace dish2 {

// base case
template<typename... Services> struct ServiceManager {

  template<typename Cell>
  static void Run( Cell& cell, const size_t, const bool alive ) {}

};

/**
 * Manages cellular service functors.
 *
 * Simulation logic is applied to each cell through a collection of distinct
 * functors, referred to as services. All services specified to run on a
 * particular update are applied in sequence to a single cell. (Some services
 * run only every $n$th update.) Then, to another randomly-chosen cell in a
 * `thread_local` population, and another until the entire population has been
 * updated.
 *
 * @note All cellular services are defined in terms of a single cell.
 * @note Dispatched by ThreadLocalServiceManager.
 * @note adapted from https://stackoverflow.com/a/35284581
 */
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
