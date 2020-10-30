#pragma once
#ifndef DISH2_CELL_SERVICES_INTERMESSAGELAUNCHINGSERVICE_HPP_INCLUDE
#define DISH2_CELL_SERVICES_INTERMESSAGELAUNCHINGSERVICE_HPP_INCLUDE

#include "../../../third-party/conduit/include/uitsl/math/shift_mod.hpp"

#include "../cell/cardinal_iterators/CpuWrapper.hpp"
#include "../config/cfg.hpp"

namespace dish2 {

struct InterMessageLaunchingService {

  static bool ShouldRun( const size_t update, const bool alive ) {
    const size_t freq = dish2::cfg.INTER_MESSAGE_LAUNCHING_SERVICE_FREQUENCY();
    return
      alive
      && freq > 0
      && uitsl::shift_mod( update, freq ) == 0;
  }

  template<typename Cell>
  static void DoService( Cell& cell ) {

    static uitsl::WarnOnce warning{
      "TODO InterMessageLaunchingService unimplemented"
    };

  }

};

} // namespace dish2

#endif // #ifndef DISH2_CELL_SERVICES_INTERMESSAGELAUNCHINGSERVICE_HPP_INCLUDE
