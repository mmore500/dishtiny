#pragma once
#ifndef DISH2_VIZ_ARTISTS_KINMATCHARTIST_HPP_INCLUDE
#define DISH2_VIZ_ARTISTS_KINMATCHARTIST_HPP_INCLUDE

#include <string>

#include "../../spec/Spec.hpp"

#include "../border_colormaps/KinGroupIDBorderColorMap.hpp"
#include "../fill_colormaps/IsAliveColorMap.hpp"
#include "../fill_colormaps/KinMatchColorMap.hpp"
#include "../getters/IsAliveGetter.hpp"
#include "../getters/KinGroupIDGetter.hpp"
#include "../getters/KinMatchGetter.hpp"
#include "../renderers/CellBorderRenderer.hpp"
#include "../renderers/CellFillRenderer.hpp"
#include "../renderers/CardinalFillRenderer.hpp"

#include "Artist.hpp"

namespace dish2 {

namespace internal::kin_match_artist {

  template<
    typename KinMatchGetter,
    typename IsAliveGetter,
    typename KinGroupIDGetter
  >
  using parent_t = dish2::Artist<
    dish2::CardinalFillRenderer<
      dish2::KinMatchColorMap,
      KinMatchGetter
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

} // namespace internal::kin_match_artist

template<
  typename KinMatchGetter=dish2::KinMatchGetter<dish2::Spec>,
  typename IsAliveGetter=dish2::IsAliveGetter<dish2::Spec>,
  typename KinGroupIDGetter=dish2::KinGroupIDGetter<dish2::Spec>
>
class KinMatchArtist
: public internal::kin_match_artist::parent_t<
  KinMatchGetter,
  IsAliveGetter,
  KinGroupIDGetter
> {

  using parent_t = internal::kin_match_artist::parent_t<
    KinMatchGetter,
    IsAliveGetter,
    KinGroupIDGetter
  >;

public:

  // inherit constructors
  using parent_t::parent_t;

  static std::string GetName() { return "Kin Match"; }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_ARTISTS_KINMATCHARTIST_HPP_INCLUDE
