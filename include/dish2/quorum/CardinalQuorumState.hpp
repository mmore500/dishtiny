#pragma once
#ifndef DISH2_QUORUM_CARDINALQUORUMSTATE_HPP_INCLUDE
#define DISH2_QUORUM_CARDINALQUORUMSTATE_HPP_INCLUDE

#include <utility>

#include "../../../third-party/conduit/include/netuit/mesh/MeshNodeInput.hpp"
#include "../../../third-party/conduit/include/netuit/mesh/MeshNodeOutput.hpp"
#include "../../../third-party/conduit/include/uitsl/algorithm/unset_mask.hpp"
#include "../../../third-party/Empirical/include/emp/base/array.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/utility/ThreadLocalRandom.hpp"

#include "CellQuorumState.hpp"

namespace dish2 {

template<typename Spec>
struct CardinalQuorumState {

  using message_t = dish2::QuorumMessage< Spec >;

  using input_t = netuit::MeshNodeInput< typename Spec::quorum_mesh_spec_t >;
  input_t input;

  using output_t = netuit::MeshNodeOutput< typename Spec::quorum_mesh_spec_t >;
  output_t output;

  message_t learned_bits{};

  size_t half_trip_counter{};

  void ForgetLearnedBits( dish2::CellQuorumState<Spec>& cell_quorum_state ) {
    cell_quorum_state.ForgetBits( learned_bits );
    learned_bits.Clear();
  }

  void DoPushBits( message_t message_bits ) {

    // remove learned bits from outgoing bits
    // this is okay for null send
    message_bits.UnsetMask( learned_bits );

    if ( output.TryPut( message_bits ) ) ++half_trip_counter;

  }

public:

  CardinalQuorumState(
    const input_t& input_,
    const output_t& output_
  ) : input( input_ )
  , output( output_ )
  {}

  size_t GetNumLearnedBits( const size_t lev ) const {
    return learned_bits.GetNumBits( lev );
  }

  size_t GetNumLearnedBits() const { return learned_bits.GetNumBits(); }

  bool GetLearnedBit( const size_t lev, const size_t bit ) const {
    return learned_bits.GetBit( lev, bit );
  }

  void ResetTripCounter() { half_trip_counter %= 2; }

  bool IsTripCounterComplete() const { return half_trip_counter >= 9; }

  bool ShouldPush() const { return half_trip_counter % 2 == 0; }

  bool ShouldPull() const { return half_trip_counter % 2 == 1; }

  void PushKnownBits(
    const CellQuorumState<Spec>& cell_quorum_state,
    const emp::array< char, Spec::NLEV > kin_match_by_lev
  ) {

    message_t message = cell_quorum_state.GetKnownBits();

    for (size_t lev{}; lev < Spec::NLEV; ++lev) {
      if ( kin_match_by_lev[lev] == false ) message.ClearLev( lev );
    }

    DoPushBits( message );

  }

  bool TryPullIncomingBits() {

    if ( input.Jump() ) {
      ++half_trip_counter;
      return true;
    } else return false;

  }

  void LearnPulledBits(
    dish2::CellQuorumState<Spec>& cell_quorum_state,
    const emp::array< char, Spec::NLEV > kin_match_by_lev
  ) {

    const message_t known_bits_before = cell_quorum_state.GetKnownBits();
    const message_t learned_bits_before = learned_bits;

    ForgetLearnedBits( cell_quorum_state );

    learned_bits = std::as_const(input).Get();

    // filter blacklisted bits from received bits
    learned_bits.UnsetMask( cell_quorum_state.GetBlacklistedBits() );

    // we didn't learn bits we already knew from somewhere else
    const message_t bits_learned_from_elsewhere = uitsl::unset_mask(
      known_bits_before, learned_bits_before
    );
    learned_bits.UnsetMask( bits_learned_from_elsewhere );

    // ignore bits from non-kin neighbors
    for (size_t lev{}; lev < Spec::NLEV; ++lev) {
      if ( kin_match_by_lev[lev] == false ) learned_bits.ClearLev( lev );
    }

    cell_quorum_state.LearnBits( learned_bits );

    // what bits did we know before that we no longer know?
    const message_t deactivated_bits = uitsl::unset_mask(
      known_bits_before, cell_quorum_state.GetKnownBits()
    );

    cell_quorum_state.BlacklistBits( deactivated_bits );

  }

};

} // namespace dish2

#endif // #ifndef DISH2_QUORUM_CARDINALQUORUMSTATE_HPP_INCLUDE
