#pragma once
#ifndef DISH2_VIZ_ARTISTS_RESOURCESTOCKPILEARTIST_HPP_INCLUDE
#define DISH2_VIZ_ARTISTS_RESOURCESTOCKPILEARTIST_HPP_INCLUDE

#include <string_view>

#include "../border_colormaps/KinGroupIDBorderColorMap.hpp"
#include "../fill_colormaps/IsAliveColorMap.hpp"
#include "../fill_colormaps/ResourceStockpileColorMap.hpp"
#include "../getters/IsAliveGetter.hpp"
#include "../getters/KinGroupIDGetter.hpp"
#include "../getters/ResourceStockpileGetter.hpp"
#include "../renderers/CellBorderRenderer.hpp"
#include "../renderers/CellFillRenderer.hpp"

#include "Artist.hpp"

namespace dish2 {

namespace internal::resource_stockpile_artist {

  template<
    typename ResourceStockpileGetter,
    typename IsAliveGetter,
    typename KinGroupIDGetter
  >
  using parent_t = dish2::Artist<
    dish2::CellFillRenderer<
      dish2::ResourceStockpileColorMap,
      ResourceStockpileGetter
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

} // namespace internal::resource_stockpile_artist

template<
  typename Spec,
  typename ResourceStockpileGetter=dish2::ResourceStockpileGetter<Spec>,
  typename IsAliveGetter=dish2::IsAliveGetter<Spec>,
  typename KinGroupIDGetter=dish2::KinGroupIDGetter<Spec>
>
class ResourceStockpileArtist
: public internal::resource_stockpile_artist::parent_t<
  ResourceStockpileGetter,
  IsAliveGetter,
  KinGroupIDGetter
> {

  using parent_t = internal::resource_stockpile_artist::parent_t<
    ResourceStockpileGetter,
    IsAliveGetter,
    KinGroupIDGetter
  >;

public:

  // inherit constructors
  using parent_t::parent_t;

  static constexpr std::string_view GetName() { return "Resource Stockpile"; }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_ARTISTS_RESOURCESTOCKPILEARTIST_HPP_INCLUDE
