#pragma once
#ifndef DISH2_VIZ_GETTERS_KINMATCHLEVGETTER_HPP_INCLUDE
#define DISH2_VIZ_GETTERS_KINMATCHLEVGETTER_HPP_INCLUDE

#include <cstddef>
#include <string_view>

#include "../../world/ThreadWorld.hpp"

#include "KinMatchGetter.hpp"

namespace dish2 {

/**
 * Whether the neighbor of a cardinal has the same kin group ID at a single
 * group level.
 */
template<typename Spec>
class KinMatchLevGetter {

  dish2::KinMatchGetter<Spec> kin_match_getter;
  size_t lev;

public:

  static constexpr std::string_view GetName() { return "KinMatchLev"; }

  using value_type = bool;

  template< typename... Args >
  KinMatchLevGetter(
    const dish2::ThreadWorld<Spec>& thread_world,
    const size_t idx,
    Args&&...
  )
  : kin_match_getter( thread_world )
  , lev( idx )
  {}

  value_type Get(
    const size_t cell_idx, const size_t cardinal_idx=0
  ) const {
    return kin_match_getter.Get( cell_idx, cardinal_idx )[ lev ];
  }

  size_t GetNumCells() const { return kin_match_getter.GetNumCells(); }

  size_t GetNumCardinals( const size_t cell_idx=0 ) const {
    return kin_match_getter.GetNumCardinals( cell_idx );
  }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_GETTERS_KINMATCHLEVGETTER_HPP_INCLUDE
