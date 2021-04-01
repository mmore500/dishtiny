#pragma once
#ifndef DISH2_VIZ_ARTISTS_DISTANCETOGRAPHCENTERARTIST_HPP_INCLUDE
#define DISH2_VIZ_ARTISTS_DISTANCETOGRAPHCENTERARTIST_HPP_INCLUDE

#include <string>

#include "../fill_colormaps/DistanceToGraphCenterColorMap.hpp"
#include "../getters/DistanceToGraphCenterGetter.hpp"
#include "../renderers/CellBorderRenderer.hpp"
#include "../renderers/CellFillRenderer.hpp"

#include "Artist.hpp"

namespace dish2 {

namespace internal::distance_to_graph_center_artist {

  template<
    typename DistanceToGraphCenterGetter
  >
  using parent_t = dish2::Artist<
    dish2::CellFillRenderer<
      dish2::DistanceToGraphCenterColorMap,
      DistanceToGraphCenterGetter
    >
  >;

} // namespace internal::distance_to_graph_center_artist

template<
  typename Spec,
  typename DistanceToGraphCenterGetter=dish2::DistanceToGraphCenterGetter<Spec>
>
class DistanceToGraphCenterArtist
: public internal::distance_to_graph_center_artist::parent_t<
  DistanceToGraphCenterGetter
> {

  using parent_t = internal::distance_to_graph_center_artist::parent_t<
    DistanceToGraphCenterGetter
  >;

public:

  // inherit constructors
  using parent_t::parent_t;

  static std::string GetName() { return "Distance to Graph Center"; }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_ARTISTS_DISTANCETOGRAPHCENTERARTIST_HPP_INCLUDE
