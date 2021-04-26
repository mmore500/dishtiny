#pragma once
#ifndef DISH2_QUORUM_CELLQUORUMSTATE_HPP_INCLUDE
#define DISH2_QUORUM_CELLQUORUMSTATE_HPP_INCLUDE

#include <algorithm>
#include <memory>
#include <utility>

#include "../../../third-party/conduit/include/uitsl/algorithm/unset_mask.hpp"
#include "../../../third-party/conduit/include/uitsl/debug/safe_compare.hpp"
#include "../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "QuorumMessage.hpp"

namespace dish2 {

template< typename Spec >
class CellQuorumState {

  using message_t = dish2::QuorumMessage< Spec >;

  message_t own_quorum_bits{ std::in_place };

  message_t known_quorum_bits{};

  /*
   * Bits that /were/ activated by a neighbor and are now deactivated are
   * temporarily blacklisted. Blacklisted bits are not accepted from any
   * neighbor. This prevents erroneous re-activation during the de-activation
   * process. Bits are blacklisted into `fresh_blacklisted_bits` and then
   * exchanged into `stale_blacklisted_bits` before being expunged. Bits that
   * are activated in `fresh_blacklisted_bits`, `stale_blacklisted_bits`, or
   * both are incorporated into the blacklisting process.
   */
  message_t fresh_blacklisted_bits{};
  message_t stale_blacklisted_bits{};

public:

  void UpdateOwnBit( const size_t lev, const size_t new_bit ) {
    ForgetBits( own_quorum_bits );
    own_quorum_bits.ClearLev( lev );
    own_quorum_bits.SetBit( lev, new_bit );
    RefreshOwnBits();
  }

  message_t GetBlacklistedBits() const {
    return fresh_blacklisted_bits | stale_blacklisted_bits;
  }

  void ProgressBlacklists() {
    stale_blacklisted_bits = std::exchange( fresh_blacklisted_bits, {} );
  }

  void BlacklistBits( const message_t bits ) { fresh_blacklisted_bits |= bits; }

  void RefreshOwnBits() { known_quorum_bits |= own_quorum_bits; }

  void ForgetBits( const message_t mask ) {
    known_quorum_bits.UnsetMask( mask );
  }

  void LearnBits( const message_t bits ) { known_quorum_bits |= bits; }

  message_t GetKnownBits() const { return known_quorum_bits; }

  size_t GetNumKnownQuorumBits( const size_t lev ) const {
    return known_quorum_bits.GetNumBits( lev );
  }

  bool GetQuorumBit( const size_t lev, const size_t bit ) const {
    return known_quorum_bits.GetBit( lev, bit );
  }

  bool GetOwnQuorumBit( const size_t lev, const size_t bit ) const {
    return own_quorum_bits.GetBit( lev, bit );
  }


};

} // namespace dish2

#endif // #ifndef DISH2_QUORUM_CELLQUORUMSTATE_HPP_INCLUDE
