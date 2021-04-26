#pragma once
#ifndef DISH2_VIZ_ARTISTS_KINGROUPAGEARTIST_HPP_INCLUDE
#define DISH2_VIZ_ARTISTS_KINGROUPAGEARTIST_HPP_INCLUDE

#include <string_view>

#include "../border_colormaps/KinGroupIDBorderColorMap.hpp"
#include "../fill_colormaps/IsAliveColorMap.hpp"
#include "../fill_colormaps/KinGroupAgeColorMap.hpp"
#include "../getters/IsAliveGetter.hpp"
#include "../getters/KinGroupAgeGetter.hpp"
#include "../getters/KinGroupIDGetter.hpp"
#include "../renderers/CellBorderRenderer.hpp"
#include "../renderers/CellFillRenderer.hpp"

#include "Artist.hpp"

namespace dish2 {

namespace internal::kin_group_age_artist {

  template<
    typename IsAliveGetter,
    typename KinGroupAgeGetter,
    typename KinGroupIDGetter
  >
  using parent_t = dish2::Artist<
    dish2::CellFillRenderer<
      dish2::KinGroupAgeColorMap,
      KinGroupAgeGetter
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

} // namespace internal::kin_group_age_artist

template<
  typename Spec,
  typename IsAliveGetter=dish2::IsAliveGetter<Spec>,
  typename KinGroupIDGetter=dish2::KinGroupIDGetter<Spec>,
  typename KinGroupAgeGetter=dish2::KinGroupAgeGetter<Spec>
>
class KinGroupAgeArtist
: public internal::kin_group_age_artist::parent_t<
  IsAliveGetter,
  KinGroupAgeGetter,
  KinGroupIDGetter
> {

  using parent_t = internal::kin_group_age_artist::parent_t<
    IsAliveGetter,
    KinGroupAgeGetter,
    KinGroupIDGetter
  >;

public:

  // inherit constructors
  using parent_t::parent_t;

  static constexpr std::string_view GetName() { return "Kin Group Age"; }

  static size_t GetSeriesLength(const dish2::ThreadWorld<Spec>&) {
    return Spec::NLEV;
  }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_ARTISTS_KINGROUPAGEARTIST_HPP_INCLUDE
