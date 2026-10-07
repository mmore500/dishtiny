#pragma once
#ifndef DISH2_VIZ_ARTISTS_CELLAGEARTIST_HPP_INCLUDE
#define DISH2_VIZ_ARTISTS_CELLAGEARTIST_HPP_INCLUDE

#include <string_view>

#include "../border_colormaps/KinGroupIDBorderColorMap.hpp"
#include "../fill_colormaps/CellAgeColorMap.hpp"
#include "../fill_colormaps/IsAliveColorMap.hpp"
#include "../getters/CellAgeGetter.hpp"
#include "../getters/IsAliveGetter.hpp"
#include "../getters/KinGroupIDGetter.hpp"
#include "../renderers/CellBorderRenderer.hpp"
#include "../renderers/CellFillRenderer.hpp"

#include "Artist.hpp"

namespace dish2 {

namespace internal::cell_age_artist {

  template<
    typename CellAgeGetter,
    typename IsAliveGetter,
    typename KinGroupIDGetter
  >
  using parent_t = dish2::Artist<
    dish2::CellFillRenderer<
      dish2::CellAgeColorMap,
      CellAgeGetter
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

} // namespace internal::cell_age_artist

template<
  typename Spec,
  typename CellAgeGetter=dish2::CellAgeGetter<Spec>,
  typename IsAliveGetter=dish2::IsAliveGetter<Spec>,
  typename KinGroupIDGetter=dish2::KinGroupIDGetter<Spec>
>
class CellAgeArtist
: public internal::cell_age_artist::parent_t<
  CellAgeGetter,
  IsAliveGetter,
  KinGroupIDGetter
> {

  using parent_t = internal::cell_age_artist::parent_t<
    CellAgeGetter,
    IsAliveGetter,
    KinGroupIDGetter
  >;

public:

  // inherit constructors
  using parent_t::parent_t;

  static constexpr std::string_view GetName() { return "Cell Age"; }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_ARTISTS_CELLAGEARTIST_HPP_INCLUDE
