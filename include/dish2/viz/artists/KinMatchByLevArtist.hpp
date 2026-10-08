#pragma once
#ifndef DISH2_VIZ_ARTISTS_KINMATCHBYLEVARTIST_HPP_INCLUDE
#define DISH2_VIZ_ARTISTS_KINMATCHBYLEVARTIST_HPP_INCLUDE

#include <string_view>

#include "../border_colormaps/MatchBorderColorMap.hpp"
#include "../fill_colormaps/BooleanColorMap.hpp"
#include "../fill_colormaps/IsAliveColorMap.hpp"
#include "../getters/IsAliveGetter.hpp"
#include "../getters/KinGroupIDLevGetter.hpp"
#include "../getters/KinMatchLevGetter.hpp"
#include "../renderers/CardinalFillRenderer.hpp"
#include "../renderers/CellBorderRenderer.hpp"
#include "../renderers/CellFillRenderer.hpp"

#include "Artist.hpp"

namespace dish2 {

namespace internal::kin_match_by_lev_artist {

  template<
    typename KinMatchLevGetter,
    typename IsAliveGetter,
    typename KinGroupIDLevGetter
  >
  using parent_t = dish2::Artist<
    dish2::CardinalFillRenderer<
      dish2::BooleanColorMap,
      KinMatchLevGetter
    >,
    dish2::CellFillRenderer<
      dish2::IsAliveColorMap,
      IsAliveGetter
    >,
    dish2::CellBorderRenderer<
      dish2::MatchBorderColorMap,
      KinGroupIDLevGetter
    >
  >;

} // namespace internal::kin_match_by_lev_artist

template<
  typename Spec,
  typename KinMatchLevGetter=dish2::KinMatchLevGetter<Spec>,
  typename IsAliveGetter=dish2::IsAliveGetter<Spec>,
  typename KinGroupIDLevGetter=dish2::KinGroupIDLevGetter<Spec>
>
class KinMatchByLevArtist
: public internal::kin_match_by_lev_artist::parent_t<
  KinMatchLevGetter,
  IsAliveGetter,
  KinGroupIDLevGetter
> {

  using parent_t = internal::kin_match_by_lev_artist::parent_t<
    KinMatchLevGetter,
    IsAliveGetter,
    KinGroupIDLevGetter
  >;

public:

  // inherit constructors
  using parent_t::parent_t;

  static constexpr std::string_view GetName() {
    return "Kin Match by Lev";
  }

  static size_t GetSeriesLength(const dish2::ThreadWorld<Spec>&) {
    return Spec::NLEV;
  }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_ARTISTS_KINMATCHBYLEVARTIST_HPP_INCLUDE
