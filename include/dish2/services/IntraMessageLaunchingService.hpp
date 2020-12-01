#pragma once
#ifndef DISH2_SERVICES_INTRAMESSAGELAUNCHINGSERVICE_HPP_INCLUDE
#define DISH2_SERVICES_INTRAMESSAGELAUNCHINGSERVICE_HPP_INCLUDE

#include "../../../third-party/conduit/include/uitsl/algorithm/for_each.hpp"
#include "../../../third-party/conduit/include/uitsl/debug/WarnOnce.hpp"
#include "../../../third-party/conduit/include/uitsl/math/shift_mod.hpp"
#include "../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "../cell/cardinal_iterators/CpuWrapper.hpp"
#include "../cell/cardinal_iterators/IncomingIntraMessageCounterWrapper.hpp"
#include "../cell/cardinal_iterators/IntraMessageNodeWrapper.hpp"
#include "../config/cfg.hpp"
#include "../debug/LogScope.hpp"

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

    const dish2::LogScope guard{ "intra message launching service", "TODO", 3 };

    using spec_t = typename Cell::spec_t;
    using tag_t = typename spec_t::tag_t;

    uitsl::for_each(
      cell.template begin< dish2::IntraMessageNodeWrapper<spec_t> >(),
      cell.template end< dish2::IntraMessageNodeWrapper<spec_t> >(),
      cell.template begin< dish2::CpuWrapper<spec_t> >(),
      cell.template begin<dish2::IncomingIntraMessageCounterWrapper<spec_t> >(),
      []( auto& im_node, auto& cpu, auto& counter ){
        thread_local emp::vector< tag_t > deduplicator;
        deduplicator.clear();

        // keep doing while any input succeeds at spawning a message
        while( std::count_if(
          std::begin( im_node.GetInputs() ), std::end( im_node.GetInputs() ),
          [&cpu, &counter]( auto& input ){

            if ( input.TryStep() == 0 ) return false;

            ++counter;

            const auto& [tag, data] = input.Get();

            // don't spawn more than one core for each tag to prevent
            // out of control execution triggering with all cardinals bcsting
            if ( std::find(
              std::begin( deduplicator ), std::end( deduplicator ), tag
            ) != std::end( deduplicator ) ) return true;
            else deduplicator.push_back( tag );

            const bool res = cpu.TryLaunchCore( tag );
            if ( res ) cpu.GetFreshestCore().SetRegisters( data );
            else counter += input.Jump();
            return res;

          }
      ) ); }
    );

  }

};

} // namespace dish2

#endif // #ifndef DISH2_SERVICES_INTRAMESSAGELAUNCHINGSERVICE_HPP_INCLUDE
