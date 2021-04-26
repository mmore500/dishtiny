#pragma once
#ifndef DISH2_VIZ_GETTERS_KINGROUPAGEGETTER_HPP_INCLUDE
#define DISH2_VIZ_GETTERS_KINGROUPAGEGETTER_HPP_INCLUDE

#include "../../../../third-party/Empirical/include/emp/datastructs/hash_utils.hpp"
#include "../../../../third-party/Empirical/include/emp/math/math.hpp"
#include "../../../../third-party/Empirical/include/emp/web/color_map.hpp"

#include "../../cell/cardinal_iterators/KinGroupAgeWrapper.hpp"
#include "../../world/ThreadWorld.hpp"

#include "CardinalIteratorAdapter.hpp"

namespace dish2 {

template<typename Spec>
class KinGroupAgeGetter {

  dish2::CardinalIteratorAdapter<Spec, dish2::KinGroupAgeWrapper> getter;

  size_t lev;

public:

  using value_type = size_t;

  template< typename... Args >
  KinGroupAgeGetter(
    const dish2::ThreadWorld<Spec>& thread_world,
    const size_t idx,
    Args&&...
  )
  : getter(thread_world)
  , lev( idx )
  {}

  const value_type Get(
    const size_t cell_idx, const size_t cardinal_idx=0
  ) const {
    return getter.Get( cell_idx, cardinal_idx ).GetBuffer()[ lev ];
  }

  size_t GetNumCells() const { return getter.GetNumCells(); }

  size_t GetNumCardinals( const size_t cell_idx=0 ) const {
    return getter.GetNumCardinals();
  }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_GETTERS_KINGROUPAGEGETTER_HPP_INCLUDE
