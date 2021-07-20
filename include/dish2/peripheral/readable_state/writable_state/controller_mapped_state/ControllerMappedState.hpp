#pragma once
#ifndef DISH2_PERIPHERAL_READABLE_STATE_WRITABLE_STATE_CONTROLLER_MAPPED_STATE_CONTROLLERMAPPEDSTATE_HPP_INCLUDE
#define DISH2_PERIPHERAL_READABLE_STATE_WRITABLE_STATE_CONTROLLER_MAPPED_STATE_CONTROLLERMAPPEDSTATE_HPP_INCLUDE

#include <cassert>

#include "../../../../../../third-party/conduit/include/uitsl/algorithm/clamp_cast.hpp"
#include "../../../../../../third-party/conduit/include/uitsl/datastructs/PodInternalNode.hpp"
#include "../../../../../../third-party/signalgp-lite/include/sgpl/utility/ByteEnumeration.hpp"
#include "../../../../../../third-party/signalgp-lite/include/sgpl/utility/ThreadLocalRandom.hpp"

#include "ApoptosisRequest.hpp"
#include "HeirRequest.hpp"
#include "RepLevRequest.hpp"
#include "ResourceReceiveResistance.hpp"
#include "ResourceReserveRequest.hpp"
#include "ResourceSendLimit.hpp"
#include "ResourceSendRequest.hpp"
#include "SpawnArrest.hpp"
#include "SpawnRequest.hpp"

namespace dish2 {

namespace internal {

template< typename Spec >
using controller_mapped_state_parent_t = uitsl::PodInternalNode<
  dish2::ApoptosisRequest,
  dish2::HeirRequest,
  dish2::RepLevRequest< Spec >,
  dish2::ResourceReceiveResistance,
  dish2::ResourceReserveRequest,
  dish2::ResourceSendLimit,
  dish2::ResourceSendRequest,
  dish2::SpawnArrest,
  dish2::SpawnRequest
>;

} // namespace internal

/**
 * Controller mapped state is a collection of simulation-controlling output
 * values that evolving programs running within each cardinal can write to and
 * read from.
 */
template< typename Spec >
struct ControllerMappedState
: public dish2::internal::controller_mapped_state_parent_t<Spec> {

  using parent_t = dish2::internal::controller_mapped_state_parent_t<Spec>;
  constexpr inline static size_t parent_size = parent_t::GetSize();

  void Assign(const size_t idx, const float val) {

    // can't use emp_assert due to obsucre macro error
    #define DISH2_CONTROLLER_MAPPED_STATE_ASSIGN_CASE_PAYLOAD(N) \
      case N: \
        if constexpr ( N  < parent_size ) { \
          using target_t = std::decay_t< decltype( \
            this->parent_t::template GetByIndex<N>() \
          ) >; \
          const target_t clamped = uitsl::clamp_cast< target_t >( val ); \
          this->parent_t::template GetByIndex<N>() = clamped; \
        } else { \
          assert( false && N ); \
          __builtin_unreachable(); \
        } \
      break;

    emp_assert( idx < parent_size );
    static_assert( parent_size < 256 );

    switch ( idx ) {

      EMP_WRAP_EACH(
        DISH2_CONTROLLER_MAPPED_STATE_ASSIGN_CASE_PAYLOAD,
        SGPL_BYTE_ENUMERATION
      )

      default:
        emp_assert( false, idx );
        __builtin_unreachable();

    }

  }

  void Randomize( const size_t idx ) {

    // can't use emp_assert due to obsucre macro error
    #define DISH2_CONTROLLER_MAPPED_STATE_RANDOMIZE_CASE_PAYLOAD(N) \
      case N: \
        if constexpr ( N  < parent_size ) { \
          auto& val = this->parent_t::template GetByIndex<N>(); \
          rng.RandFill( \
            reinterpret_cast<unsigned char*>(&val), sizeof( val ) \
          ); \
        } else { \
          assert( false && N ); \
          __builtin_unreachable(); \
        } \
      break;

    emp_assert( idx < parent_size );
    static_assert( parent_size < 256 );

    auto& rng = sgpl::tlrand.Get();

    switch ( idx ) {

      EMP_WRAP_EACH(
        DISH2_CONTROLLER_MAPPED_STATE_RANDOMIZE_CASE_PAYLOAD,
        SGPL_BYTE_ENUMERATION
      )

      default:
        emp_assert( false, idx );
        __builtin_unreachable();

    }

  }

};

} // namespace dish2

#endif // #ifndef DISH2_PERIPHERAL_READABLE_STATE_WRITABLE_STATE_CONTROLLER_MAPPED_STATE_CONTROLLERMAPPEDSTATE_HPP_INCLUDE
