#pragma once
#ifndef DISH2_SERVICES_INTERMESSAGELAUNCHINGSERVICE_HPP_INCLUDE
#define DISH2_SERVICES_INTERMESSAGELAUNCHINGSERVICE_HPP_INCLUDE

#include "../../../third-party/conduit/include/uitsl/algorithm/for_each.hpp"
#include "../../../third-party/conduit/include/uitsl/math/shift_mod.hpp"

#include "../cell/cardinal_iterators/CpuWrapper.hpp"
#include "../cell/cardinal_iterators/IncomingInterMessageCounterWrapper.hpp"
#include "../cell/cardinal_iterators/MessageNodeInputWrapper.hpp"
#include "../config/cfg.hpp"
#include "../debug/LogScope.hpp"

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

    const dish2::LogScope guard{ "inter message launching service", "TODO", 3 };

    using spec_t = typename Cell::spec_t;

    uitsl::for_each(
      cell.template begin< dish2::MessageNodeInputWrapper<spec_t> >(),
      cell.template end< dish2::MessageNodeInputWrapper<spec_t> >(),
      cell.template begin< dish2::CpuWrapper<spec_t> >(),
      cell.template begin<dish2::IncomingInterMessageCounterWrapper<spec_t> >(),
      []( auto& message_input, auto& cpu, auto& message_counter ) {
        while(
          message_input.TryStep()
          && cpu.TryLaunchCore( std::get<0>( message_input.Get() ), 1)
        ) {
          cpu.GetFreshestCore().SetRegisters(
            std::get<1>( message_input.Get() )
          );
          ++message_counter;
        }
        // leftover messages purged in a seperate service
        // everyone must jump at once because it's an aggregated duct
      }
    );

  }

};

} // namespace dish2

#endif // #ifndef DISH2_SERVICES_INTERMESSAGELAUNCHINGSERVICE_HPP_INCLUDE
