#pragma once
#ifndef DISH2_SERVICES_INTRAMESSAGELAUNCHINGSERVICE_HPP_INCLUDE
#define DISH2_SERVICES_INTRAMESSAGELAUNCHINGSERVICE_HPP_INCLUDE

#include "../../../third-party/conduit/include/uitsl/algorithm/for_each.hpp"
#include "../../../third-party/conduit/include/uitsl/debug/WarnOnce.hpp"
#include "../../../third-party/conduit/include/uitsl/math/shift_mod.hpp"

#include "../cell/cardinal_iterators/CpuWrapper.hpp"
#include "../cell/cardinal_iterators/IntraMessageNodeWrapper.hpp"
#include "../config/cfg.hpp"

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
  static void DoService( Cell& cell, const size_t ) {

    using spec_t = typename Cell::spec_t;

    uitsl::for_each(
      cell.template begin< dish2::IntraMessageNodeWrapper<spec_t> >(),
      cell.template end< dish2::IntraMessageNodeWrapper<spec_t> >(),
      cell.template begin< dish2::CpuWrapper<spec_t> >(),
      []( auto& im_node, auto& cpu ){ while( std::count_if(
        std::begin( im_node.GetInputs() ),
        std::end( im_node.GetInputs() ),
        [&cpu]( auto& input ){
          if ( input.TryStep() ) return cpu.TryLaunchCore( input.Get() );
          else return false;
        }
      ) ); }
    );

  }

};

} // namespace dish2

#endif // #ifndef DISH2_SERVICES_INTRAMESSAGELAUNCHINGSERVICE_HPP_INCLUDE
