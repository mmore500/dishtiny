#pragma once
#ifndef DISH2_SERVICES_INTERMESSAGELAUNCHINGSERVICE_HPP_INCLUDE
#define DISH2_SERVICES_INTERMESSAGELAUNCHINGSERVICE_HPP_INCLUDE

#include "../../../third-party/conduit/include/uitsl/algorithm/for_each.hpp"
#include "../../../third-party/conduit/include/uitsl/math/shift_mod.hpp"

#include "../cell/cardinal_iterators/CpuWrapper.hpp"
#include "../cell/cardinal_iterators/IncomingInterMessageCounterWrapper.hpp"
#include "../cell/cardinal_iterators/MessageNodeInputWrapper.hpp"
#include "../cell/cardinal_iterators/PeripheralWrapper.hpp"
#include "../config/cfg.hpp"
#include "../debug/LogScope.hpp"

namespace dish2 {

/**
 * Launches new virtual cores to process incoming inter-cell messages.
 */
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
      cell.template begin< dish2::PeripheralWrapper<spec_t> >(),
      cell.template begin<dish2::IncomingInterMessageCounterWrapper<spec_t> >(),
      [](
        auto& message_input, auto& cpu,
        auto& peripheral, auto& message_counter
      ) {

        auto& selfsend_buffer = peripheral.inter_message_selfsend_buffer;

        const auto try_launch = [&]( const auto& message ){

          const auto& [tag, data] = message;
          const bool res = cpu.TryLaunchCore( tag, 1 );
          ++message_counter;
          if (res) {
            cpu.GetFreshestCore().SetRegisters( data );
          }
          return res;

        };

        // interleave selfsend messages while there are any of either
        while(
          message_input.TryStep() && try_launch( message_input.Get() )
          + (
            selfsend_buffer.size()
            && (
              try_launch( selfsend_buffer.front() )
              + [&](){ selfsend_buffer.pop_front(); return 0; }()
            )
          )
        );

        message_counter += selfsend_buffer.size();
        selfsend_buffer.clear();

        // non-self-send leftover messages purged in a seperate service
        // everyone must jump at once because it's an aggregated duct
      }
    );

  }

};

} // namespace dish2

#endif // #ifndef DISH2_SERVICES_INTERMESSAGELAUNCHINGSERVICE_HPP_INCLUDE
