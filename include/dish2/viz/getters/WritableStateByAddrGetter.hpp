#pragma once
#ifndef DISH2_VIZ_GETTERS_WRITABLESTATEBYADDRGETTER_HPP_INCLUDE
#define DISH2_VIZ_GETTERS_WRITABLESTATEBYADDRGETTER_HPP_INCLUDE

#include <cstddef>
#include <string_view>

#include "../../cell/cardinal_iterators/ReadableStateWrapper.hpp"
#include "../../peripheral/readable_state/introspective_state/IntrospectiveState.hpp"
#include "../../peripheral/readable_state/writable_state/WritableState.hpp"
#include "../../world/ThreadWorld.hpp"

#include "CardinalIteratorAdapter.hpp"

namespace dish2 {

/**
 * Value of a single writable state register of a cardinal.
 */
template<typename Spec>
class WritableStateByAddrGetter {

  dish2::CardinalIteratorAdapter<
    Spec, dish2::ReadableStateWrapper
  > readable_state_getter;

  size_t addr;

public:

  static constexpr std::string_view GetName() {
    return "WritableStateByAddr";
  }

  using value_type = double;

  template< typename... Args >
  WritableStateByAddrGetter(
    const dish2::ThreadWorld<Spec>& thread_world_,
    const size_t idx,
    Args&&...
  )
  : readable_state_getter(thread_world_)
  , addr( idx )
  {}

  value_type Get(
    const size_t cell_idx, const size_t cardinal_idx=0
  ) const {
    // writable state follows introspective state in the readable state
    // address space
    return readable_state_getter.Get( cell_idx, cardinal_idx ).Read(
      dish2::IntrospectiveState<Spec>::GetSize() + addr
    );
  }

  size_t GetNumCells() const { return readable_state_getter.GetNumCells(); }

  size_t GetNumCardinals( const size_t cell_idx=0 ) const {
    return readable_state_getter.GetNumCardinals( cell_idx );
  }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_GETTERS_WRITABLESTATEBYADDRGETTER_HPP_INCLUDE
