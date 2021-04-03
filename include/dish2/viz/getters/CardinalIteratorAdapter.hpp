#pragma once
#ifndef DISH2_VIZ_GETTERS_CARDINALITERATORADAPTER_HPP_INCLUDE
#define DISH2_VIZ_GETTERS_CARDINALITERATORADAPTER_HPP_INCLUDE

#include <functional>

#include "../../../../third-party/Empirical/include/emp/datastructs/hash_utils.hpp"
#include "../../../../third-party/Empirical/include/emp/math/math.hpp"
#include "../../../../third-party/Empirical/include/emp/web/color_map.hpp"

#include "../../world/ThreadWorld.hpp"

namespace dish2 {

template<typename Spec, template<typename> typename CardinalIterator>
class CardinalIteratorAdapter {

  std::reference_wrapper<const dish2::ThreadWorld<Spec>> thread_world;

public:

  using value_type = typename CardinalIterator<Spec>::value_type;

  template< typename... Args >
  CardinalIteratorAdapter(
    const dish2::ThreadWorld<Spec>& thread_world_,
    Args&&...
  )
  : thread_world(thread_world_)
  {}

  decltype(auto) Get(
    const size_t cell_idx, const size_t cardinal_idx=0
  ) const {
    const auto begin_it = thread_world.get().GetCell(cell_idx).template begin<
      CardinalIterator<Spec>
    >();
    return *std::next( begin_it, cardinal_idx );
  }

  // TODO expose begin and end instead of these for getters
  size_t GetNumCells() const { return thread_world.get().GetSize(); }

  size_t GetNumCardinals( const size_t cell_idx=0 ) const {
    return thread_world.get().GetCell( cell_idx ).GetNumCardinals();
  }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_GETTERS_CARDINALITERATORADAPTER_HPP_INCLUDE
