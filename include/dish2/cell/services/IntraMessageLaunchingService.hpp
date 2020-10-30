#pragma once
#ifndef DISH2_CELL_SERVICES_INTRAMESSAGELAUNCHINGSERVICE_HPP_INCLUDE
#define DISH2_CELL_SERVICES_INTRAMESSAGELAUNCHINGSERVICE_HPP_INCLUDE

#include "../../../../third-party/conduit/include/uitsl/debug/WarnOnce.hpp"
#include "../../../../third-party/conduit/include/uitsl/math/shift_mod.hpp"

#include "../../config/cfg.hpp"

namespace dish2 {

struct IntraMessageLaunchingService {

  static bool ShouldRun( const size_t update, const bool alive ) {
    const size_t freq = dish2::cfg.INTRA_MESSAGE_LAUNCHING_SERVICE_FREQUENCY();
    return
      alive
      && freq > 0
      && uitsl::shift_mod( update, freq ) == 0;
  }

  template<typename Cell>
  static void DoService( Cell& cell ) {

    static uitsl::WarnOnce warning{
      "TODO IntraMessageLaunchingService unimplemented"
    };

  }

};

} // namespace dish2

#endif // #ifndef DISH2_CELL_SERVICES_INTRAMESSAGELAUNCHINGSERVICE_HPP_INCLUDE
