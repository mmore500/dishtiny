#pragma once
#ifndef DISH2_VIZ_GETTERS_EXTROSPECTIVESTATEBYADDRGETTER_HPP_INCLUDE
#define DISH2_VIZ_GETTERS_EXTROSPECTIVESTATEBYADDRGETTER_HPP_INCLUDE

#include <cstddef>
#include <string_view>

#include "../../cell/cardinal_iterators/ExtrospectiveStateWrapper.hpp"
#include "../../peripheral/readable_state/introspective_state/IntrospectiveState.hpp"
#include "../../world/ThreadWorld.hpp"

#include "CardinalIteratorAdapter.hpp"

namespace dish2 {

/**
 * Value of a single extrospective state register of a cardinal.
 */
template<typename Spec>
class ExtrospectiveStateByAddrGetter {

  dish2::CardinalIteratorAdapter<
    Spec, dish2::ExtrospectiveStateWrapper
  > extrospective_state_getter;

  size_t addr;

public:

  static constexpr std::string_view GetName() {
    return "ExtrospectiveStateByAddr";
  }

  static constexpr size_t GetNumAddrs() {
    return dish2::IntrospectiveState<Spec>::GetSize();
  }

  using value_type = double;

  template< typename... Args >
  ExtrospectiveStateByAddrGetter(
    const dish2::ThreadWorld<Spec>& thread_world_,
    const size_t idx,
    Args&&...
  )
  : extrospective_state_getter(thread_world_)
  , addr( idx )
  {}

  value_type Get(
    const size_t cell_idx, const size_t cardinal_idx=0
  ) const {
    return extrospective_state_getter.Get( cell_idx, cardinal_idx ).Read( addr );
  }

  size_t GetNumCells() const { return extrospective_state_getter.GetNumCells(); }

  size_t GetNumCardinals( const size_t cell_idx=0 ) const {
    return extrospective_state_getter.GetNumCardinals( cell_idx );
  }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_GETTERS_EXTROSPECTIVESTATEBYADDRGETTER_HPP_INCLUDE
