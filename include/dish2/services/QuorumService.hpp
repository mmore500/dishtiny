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

/**
 * Performs a distributed estimation of kin group size.
 *
 * Each cell has a single randomly-chosen index set within a fixed-length
 * bitstring. (Depending in parameter settings, some cells may may have index
 * set --- all positions within the bitstring are zeroed out.)
 *
 * Broadcasts bits known to be set are to all neighbor cells within the same
 * kin group. Incoming bitstrings from neighbors are ORed with known bits.
 *
 * The original neighbor each non-self bit was first learned from is recorded
 * alongside that bit. If that neighbor no longer broadcasts that bit, it is
 * erased from the cell's known bits.
 *
 * Updates latest quorum count into introspective state.
 *
 * This scheme is replicated independently for each kin group level simulated.
 */
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

    // temporary blacklists prevent bits
    // that the source /we/ learned them from
    // has forgotten about
    // from being reactivated by neighbors
    // /we/ taught them to
    // ... move the process of purging them
    // forward
    ProgressBlacklists<Cell>( cell );

    // add any quorum bits that neighbors
    // have learned about to known bits
    UpdateKnownBits<Cell>( cell );

    // share all our known bits with neighbors
    PushKnownBits<Cell>( cell );

    // so that cpus can sense num quorum bits
    UpdateIntrospectiveState<Cell>( cell );

  }

};

} // namespace dish2

#endif // #ifndef DISH2_SERVICES_QUORUMSERVICE_HPP_INCLUDE
