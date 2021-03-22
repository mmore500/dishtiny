#pragma once
#ifndef DISH2_VIZ_ARTISTS_SPAWNEDFROMARTIST_HPP_INCLUDE
#define DISH2_VIZ_ARTISTS_SPAWNEDFROMARTIST_HPP_INCLUDE

#include <string>

#include "../border_colormaps/KinGroupIDBorderColorMap.hpp"
#include "../fill_colormaps/BooleanColorMap.hpp"
#include "../fill_colormaps/IsAliveColorMap.hpp"
#include "../getters/IsAliveGetter.hpp"
#include "../getters/KinGroupIDGetter.hpp"
#include "../getters/SpawnedFromGetter.hpp"
#include "../renderers/CardinalFillRenderer.hpp"
#include "../renderers/CellBorderRenderer.hpp"
#include "../renderers/CellFillRenderer.hpp"

#include "Artist.hpp"

namespace dish2 {

namespace internal::spawned_from_artist {

  template<
    typename SpawnedFromGetter,
    typename IsAliveGetter,
    typename KinGroupIDGetter
  >
  using parent_t = dish2::Artist<
    dish2::CardinalFillRenderer<
      dish2::BooleanColorMap,
      SpawnedFromGetter
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

} // namespace internal::spawned_from_artist

template<
  typename Spec,
  typename SpawnedFromGetter=dish2::SpawnedFromGetter<Spec>,
  typename IsAliveGetter=dish2::IsAliveGetter<Spec>,
  typename KinGroupIDGetter=dish2::KinGroupIDGetter<Spec>
>
class SpawnedFromArtist
: public internal::spawned_from_artist::parent_t<
  SpawnedFromGetter,
  IsAliveGetter,
  KinGroupIDGetter
> {

  using parent_t = internal::spawned_from_artist::parent_t<
    SpawnedFromGetter,
    IsAliveGetter,
    KinGroupIDGetter
  >;

public:

  // inherit constructors
  using parent_t::parent_t;

  static std::string GetName() { return "Spawned From"; }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_ARTISTS_SPAWNEDFROMARTIST_HPP_INCLUDE
