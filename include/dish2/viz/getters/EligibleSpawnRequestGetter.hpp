#pragma once
#ifndef DISH2_VIZ_GETTERS_ELIGIBLESPAWNREQUESTGETTER_HPP_INCLUDE
#define DISH2_VIZ_GETTERS_ELIGIBLESPAWNREQUESTGETTER_HPP_INCLUDE

#include <cstddef>
#include <string_view>
#include <tuple>

#include "../../world/ThreadWorld.hpp"

#include "SpawnArrestGetter.hpp"
#include "SpawnRequestGetter.hpp"

namespace dish2 {

/**
 * Whether a cardinal has a spawn request and whether it has a spawn arrest, as
 * (requested, arrested).
 */
template<typename Spec>
class EligibleSpawnRequestGetter {

  dish2::SpawnRequestGetter<Spec> request_getter;
  dish2::SpawnArrestGetter<Spec> arrest_getter;

public:

  static constexpr std::string_view GetName() { return "EligibleSpawnRequest"; }

  using value_type = std::tuple<bool, bool>;

  template< typename... Args >
  EligibleSpawnRequestGetter( const dish2::ThreadWorld<Spec>& tw, Args&&... )
  : request_getter( tw )
  , arrest_getter( tw )
  {}

  value_type Get( const size_t cell_idx, const size_t cardinal_idx=0 ) const {
    return {
      static_cast<bool>( request_getter.Get( cell_idx, cardinal_idx ) ),
      static_cast<bool>( arrest_getter.Get( cell_idx, cardinal_idx ) )
    };
  }

  size_t GetNumCells() const { return request_getter.GetNumCells(); }

  size_t GetNumCardinals( const size_t cell_idx=0 ) const {
    return request_getter.GetNumCardinals( cell_idx );
  }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_GETTERS_ELIGIBLESPAWNREQUESTGETTER_HPP_INCLUDE
