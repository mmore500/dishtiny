#pragma once
#ifndef DISH2_SERVICES_QUORUMSERVICE_HPP_INCLUDE
#define DISH2_SERVICES_QUORUMSERVICE_HPP_INCLUDE

#include <algorithm>

#include "../../../third-party/conduit/include/uitsl/algorithm/for_each.hpp"
#include "../../../third-party/conduit/include/uitsl/math/shift_mod.hpp"
#include "../../../third-party/conduit/include/uitsl/polyfill/identity.hpp"

#include "../cell/cardinal_iterators/CardinalQuorumStateWrapper.hpp"
#include "../cell/cardinal_iterators/KinMatchWrapper.hpp"
#include "../cell/cardinal_iterators/NumKnownQuorumBitsWrapper.hpp"
#include "../config/cfg.hpp"
#include "../debug/LogScope.hpp"

namespace dish2 {

class QuorumService {

  template<typename Cell>
  static void ProgressBlacklists( Cell& cell ) {

    using spec_t = typename Cell::spec_t;

    const bool all_cardinals_full_trip = std::all_of(
      cell.template begin< dish2::CardinalQuorumStateWrapper<spec_t> >(),
      cell.template end< dish2::CardinalQuorumStateWrapper<spec_t> >(),
      []( const auto& cardinal ){ return cardinal.IsTripCounterComplete(); }
    );

    if ( all_cardinals_full_trip ) {
      std::for_each(
        cell.template begin< dish2::CardinalQuorumStateWrapper<spec_t> >(),
        cell.template end< dish2::CardinalQuorumStateWrapper<spec_t> >(),
        []( auto& cardinal ){ cardinal.ResetTripCounter(); }
      );
      cell.cell_quorum_state.ProgressBlacklists();
    }

  }

  template<typename Cell>
  static void UpdateKnownBits( Cell& cell ) {

    const dish2::LogScope guard{ "quorum service", "TODO", 3 };

    using spec_t = typename Cell::spec_t;

    auto& cell_quorum_state = cell.cell_quorum_state;

    uitsl::for_each(
      cell.template begin< dish2::CardinalQuorumStateWrapper<spec_t> >(),
      cell.template end< dish2::CardinalQuorumStateWrapper<spec_t> >(),
      cell.template begin< dish2::KinMatchWrapper<spec_t> >(),
      [&cell_quorum_state]( auto& cardinal, const auto kin_match_array ){
        if ( cardinal.ShouldPull() ) {
          if ( cardinal.TryPullIncomingBits() ) {
            cardinal.LearnPulledBits( cell_quorum_state, kin_match_array );
          }
        }
      }
    );

  }

  template<typename Cell>
  static void PushKnownBits( Cell& cell ) {

    using spec_t = typename Cell::spec_t;

    auto& cell_quorum_state = cell.cell_quorum_state;

    uitsl::for_each(
      cell.template begin<dish2::CardinalQuorumStateWrapper<spec_t>>(),
      cell.template end<dish2::CardinalQuorumStateWrapper<spec_t>>(),
      cell.template begin<dish2::KinMatchWrapper<spec_t>>(),
      [&cell_quorum_state]( auto& cardinal, const auto kin_match_array ){
        if ( cardinal.ShouldPush() ) {
          cardinal.PushKnownBits( cell_quorum_state, kin_match_array );
        }
      }
    );

  }

  template<typename Cell>
  static void UpdateIntrospectiveState( Cell& cell ) {

    using spec_t = typename Cell::spec_t;

    auto& cell_quorum_state = cell.cell_quorum_state;

    std::for_each(
      cell.template begin<dish2::NumKnownQuorumBitsWrapper<spec_t>>(),
      cell.template end<dish2::NumKnownQuorumBitsWrapper<spec_t>>(),
      [&cell_quorum_state]( auto& num_quorum_bits_state ){
        for (size_t lev{}; lev < spec_t::NLEV; ++lev) {
          num_quorum_bits_state.Get( lev )
            = cell_quorum_state.GetNumKnownQuorumBits( lev );
        }
      }
    );

  }

public:

  static bool ShouldRun( const size_t update, const bool alive ) {
    const size_t freq = dish2::cfg.QUORUM_SERVICE_FREQUENCY();
    return
      freq > 0
      && uitsl::shift_mod( update, freq ) == 0;
  }

  template<typename Cell>
  static void DoService( Cell& cell ) {

    cell.cell_quorum_state.RefreshOwnBits();

    ProgressBlacklists<Cell>( cell );

    UpdateKnownBits<Cell>( cell );

    PushKnownBits<Cell>( cell );

    // so that cpus can sense num quorum bits
    UpdateIntrospectiveState<Cell>( cell );

  }

};

} // namespace dish2

#endif // #ifndef DISH2_SERVICES_QUORUMSERVICE_HPP_INCLUDE
