#pragma once
#ifndef DISH2_VIZ_ARTISTS_NEIGHBORPOSARTIST_HPP_INCLUDE
#define DISH2_VIZ_ARTISTS_NEIGHBORPOSARTIST_HPP_INCLUDE

#include <string_view>

#include "../border_colormaps/KinGroupIDBorderColorMap.hpp"
#include "../fill_colormaps/IsAliveColorMap.hpp"
#include "../fill_colormaps/QuorumBitsColorMap.hpp"
#include "../getters/KinGroupIDGetter.hpp"
#include "../getters/NeighborPosGetter.hpp"
#include "../renderers/CardinalFillRenderer.hpp"
#include "../renderers/CellBorderRenderer.hpp"
#include "../renderers/CellFillRenderer.hpp"

#include "Artist.hpp"

namespace dish2 {

namespace internal::neighbor_pos_artist {

  template<
    typename KinGroupIDGetter,
    typename NeighborPosGetter
  >
  using parent_t = dish2::Artist<
    dish2::CardinalFillRenderer<
      dish2::QuorumBitsColorMap,
      NeighborPosGetter
    >,
    dish2::CellBorderRenderer<
      dish2::KinGroupIDBorderColorMap,
      KinGroupIDGetter
    >
  >;

} // namespace internal::neighbor_pos_artist

template<
  typename Spec,
  typename KinGroupIDGetter=dish2::KinGroupIDGetter<Spec>,
  typename NeighborPosGetter=dish2::NeighborPosGetter<Spec>
>
class NeighborPosArtist
: public internal::neighbor_pos_artist::parent_t<
  KinGroupIDGetter,
  NeighborPosGetter
> {

  using parent_t = internal::neighbor_pos_artist::parent_t<
    KinGroupIDGetter,
    NeighborPosGetter
    >;

public:

  // inherit constructors
  using parent_t::parent_t;

  static constexpr std::string_view GetName() { return "Neighbor Pos"; }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_ARTISTS_NEIGHBORPOSARTIST_HPP_INCLUDE
