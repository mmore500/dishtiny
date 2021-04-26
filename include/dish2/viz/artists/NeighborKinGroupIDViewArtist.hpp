#pragma once
#ifndef DISH2_VIZ_ARTISTS_NEIGHBORKINGROUPIDVIEWARTIST_HPP_INCLUDE
#define DISH2_VIZ_ARTISTS_NEIGHBORKINGROUPIDVIEWARTIST_HPP_INCLUDE

#include <string_view>

#include "../border_colormaps/KinGroupIDBorderColorMap.hpp"
#include "../fill_colormaps/IsAliveColorMap.hpp"
#include "../fill_colormaps/KinGroupIDFillColorMap.hpp"
#include "../getters/IsAliveGetter.hpp"
#include "../getters/KinGroupIDGetter.hpp"
#include "../getters/NeighborKinGroupIDViewGetter.hpp"
#include "../renderers/CardinalFillRenderer.hpp"
#include "../renderers/CellBorderRenderer.hpp"
#include "../renderers/CellFillRenderer.hpp"

#include "Artist.hpp"

namespace dish2 {

namespace internal::neighbor_kin_group_id_view_artist {

  template<
    typename IsAliveGetter,
    typename KinGroupIDGetter,
    typename NeighborKinGroupIDViewGetter
  >
  using parent_t = dish2::Artist<
    dish2::CardinalFillRenderer<
      dish2::KinGroupIDFillColorMap,
      NeighborKinGroupIDViewGetter
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

} // namespace internal::neighbor_kin_group_id_view_artist

template<
  typename Spec,
  typename IsAliveGetter=dish2::IsAliveGetter<Spec>,
  typename KinGroupIDGetter=dish2::KinGroupIDGetter<Spec>,
  typename NeighborKinGroupIDViewGetter
    =dish2::NeighborKinGroupIDViewGetter<Spec>
>
class NeighborKinGroupIDViewArtist
: public internal::neighbor_kin_group_id_view_artist::parent_t<
  IsAliveGetter,
  KinGroupIDGetter,
  NeighborKinGroupIDViewGetter
> {

  using parent_t = internal::neighbor_kin_group_id_view_artist::parent_t<
    IsAliveGetter,
    KinGroupIDGetter,
    NeighborKinGroupIDViewGetter
    >;

public:

  // inherit constructors
  using parent_t::parent_t;

  static constexpr std::string_view GetName() { return "Neighbor Kin Group ID"; }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_ARTISTS_NEIGHBORKINGROUPIDVIEWARTIST_HPP_INCLUDE
