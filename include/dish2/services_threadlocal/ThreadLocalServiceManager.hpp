#pragma once
#ifndef DISH2_SERVICES_THREADLOCAL_THREADLOCALSERVICEMANAGER_HPP_INCLUDE
#define DISH2_SERVICES_THREADLOCAL_THREADLOCALSERVICEMANAGER_HPP_INCLUDE

namespace dish2 {

// base case
template<typename... Services> struct ThreadLocalServiceManager {

  template<typename ThreadWorld>
  static void Run(ThreadWorld& thread_world, const size_t) {}

};

/**
 * Manages thread_local service functors.
 *
 * Simulation logic arises from sequential application of functors to a
 * thread_local population.
 *
 * @note All thread_local services are defined in terms of `ThreadWorld`, a
 * thread-local of population of cells.
 * @note Dispatches ServiceManager.
 * @note adapted from https://stackoverflow.com/a/35284581
 */

// adapted from https://stackoverflow.com/a/35284581
template<typename FirstService, typename... SubsequentServices>
struct ThreadLocalServiceManager<FirstService, SubsequentServices...> {

  template<typename ThreadWorld>
  static void Run( ThreadWorld& thread_world, const size_t update ) {

    using subsequent_services_t = dish2::ThreadLocalServiceManager<SubsequentServices...>;

    if ( FirstService::ShouldRun( update ) ) {
      FirstService::DoService( thread_world, update );
    }

    if constexpr ( sizeof...(SubsequentServices) > 0 ) {
      subsequent_services_t::template Run<ThreadWorld>( thread_world, update );
    }

  }

};

} // namespace dish2

#endif // #ifndef DISH2_SERVICES_THREADLOCAL_THREADLOCALSERVICEMANAGER_HPP_INCLUDE
