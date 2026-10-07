#pragma once
#ifndef DISH2_VIZ_ARTISTS_ELIGIBLESPAWNREQUESTARTIST_HPP_INCLUDE
#define DISH2_VIZ_ARTISTS_ELIGIBLESPAWNREQUESTARTIST_HPP_INCLUDE

#include <string_view>

#include "../border_colormaps/KinGroupIDBorderColorMap.hpp"
#include "../fill_colormaps/EligibleSpawnRequestColorMap.hpp"
#include "../fill_colormaps/IsAliveColorMap.hpp"
#include "../getters/EligibleSpawnRequestGetter.hpp"
#include "../getters/IsAliveGetter.hpp"
#include "../getters/KinGroupIDGetter.hpp"
#include "../renderers/CardinalFillRenderer.hpp"
#include "../renderers/CellBorderRenderer.hpp"
#include "../renderers/CellFillRenderer.hpp"

#include "Artist.hpp"

namespace dish2 {

namespace internal::eligible_spawn_request_artist {

  template<
    typename EligibleSpawnRequestGetter,
    typename IsAliveGetter,
    typename KinGroupIDGetter
  >
  using parent_t = dish2::Artist<
    dish2::CardinalFillRenderer<
      dish2::EligibleSpawnRequestColorMap,
      EligibleSpawnRequestGetter
    >,
    dish2::CellFillRenderer<
      dish2::IsAliveColorMap,
      IsAliveGetter
    >,
    dish2::CellBorderRenderer<
      dish2::KinGroupIDBorderColorMap,
      KinGroupIDGetter
    >
  >;

} // namespace internal::eligible_spawn_request_artist

template<
  typename Spec,
  typename EligibleSpawnRequestGetter=dish2::EligibleSpawnRequestGetter<Spec>,
  typename IsAliveGetter=dish2::IsAliveGetter<Spec>,
  typename KinGroupIDGetter=dish2::KinGroupIDGetter<Spec>
>
class EligibleSpawnRequestArtist
: public internal::eligible_spawn_request_artist::parent_t<
  EligibleSpawnRequestGetter,
  IsAliveGetter,
  KinGroupIDGetter
> {

  using parent_t = internal::eligible_spawn_request_artist::parent_t<
    EligibleSpawnRequestGetter,
    IsAliveGetter,
    KinGroupIDGetter
  >;

public:

  // inherit constructors
  using parent_t::parent_t;

  static constexpr std::string_view GetName() { return "Eligible Spawn Request"; }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_ARTISTS_ELIGIBLESPAWNREQUESTARTIST_HPP_INCLUDE
