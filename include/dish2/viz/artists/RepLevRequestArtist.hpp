#pragma once
#ifndef DISH2_VIZ_ARTISTS_REPLEVREQUESTARTIST_HPP_INCLUDE
#define DISH2_VIZ_ARTISTS_REPLEVREQUESTARTIST_HPP_INCLUDE

#include <string_view>

#include "../border_colormaps/KinGroupIDBorderColorMap.hpp"
#include "../fill_colormaps/RepLevRequestColorMap.hpp"
#include "../fill_colormaps/IsAliveColorMap.hpp"
#include "../getters/RepLevRequestGetter.hpp"
#include "../getters/IsAliveGetter.hpp"
#include "../getters/KinGroupIDGetter.hpp"
#include "../renderers/CardinalFillRenderer.hpp"
#include "../renderers/CellBorderRenderer.hpp"
#include "../renderers/CellFillRenderer.hpp"

#include "Artist.hpp"

namespace dish2 {

namespace internal::rep_lev_request_artist {

  template<
    typename RepLevRequestGetter,
    typename IsAliveGetter,
    typename KinGroupIDGetter
  >
  using parent_t = dish2::Artist<
    dish2::CardinalFillRenderer<
      dish2::RepLevRequestColorMap,
      RepLevRequestGetter
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

} // namespace internal::rep_lev_request_artist

template<
  typename Spec,
  typename RepLevRequestGetter=dish2::RepLevRequestGetter<Spec>,
  typename IsAliveGetter=dish2::IsAliveGetter<Spec>,
  typename KinGroupIDGetter=dish2::KinGroupIDGetter<Spec>
>
class RepLevRequestArtist
: public internal::rep_lev_request_artist::parent_t<
  RepLevRequestGetter,
  IsAliveGetter,
  KinGroupIDGetter
> {

  using parent_t = internal::rep_lev_request_artist::parent_t<
    RepLevRequestGetter,
    IsAliveGetter,
    KinGroupIDGetter
  >;

public:

  // inherit constructors
  using parent_t::parent_t;

  static constexpr std::string_view GetName() { return "Rep Lev Request"; }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_ARTISTS_REPLEVREQUESTARTIST_HPP_INCLUDE
