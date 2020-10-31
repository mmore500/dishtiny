#pragma once
#ifndef DISH2_SERVICES_EVENTLAUNCHINGSERVICE_HPP_INCLUDE
#define DISH2_SERVICES_EVENTLAUNCHINGSERVICE_HPP_INCLUDE

#include <algorithm>

#include "../../../third-party/conduit/include/uitsl/debug/WarnOnce.hpp"
#include "../../../third-party/conduit/include/uitsl/math/shift_mod.hpp"

#include "../cell/cardinal_iterators/CpuWrapper.hpp"
#include "../config/cfg.hpp"

namespace dish2 {

struct EventLaunchingService {

  static bool ShouldRun( const size_t update, const bool alive ) {
    const size_t freq = dish2::cfg.EVENT_LAUNCHING_SERVICE_FREQUENCY();
    return
      alive
      && freq > 0
      && uitsl::shift_mod( update, freq ) == 0;
  }

  template<typename Cell>
  static void DoService( Cell& cell ) {

    std::for_each(
      cell.template begin<dish2::CpuWrapper<Spec>>(),
      cell.template end<dish2::CpuWrapper<Spec>>(),
      [](auto& cpu){ cpu.TryLaunchCore(); }
    );

    static uitsl::WarnOnce warning{"TODO EventLaunchingService unimplemented"};

  }

};

} // namespace dish2

#endif // #ifndef DISH2_SERVICES_EVENTLAUNCHINGSERVICE_HPP_INCLUDE
