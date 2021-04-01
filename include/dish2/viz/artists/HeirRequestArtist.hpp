#pragma once
#ifndef DISH2_VIZ_ARTISTS_HEIRREQUESTARTIST_HPP_INCLUDE
#define DISH2_VIZ_ARTISTS_HEIRREQUESTARTIST_HPP_INCLUDE

#include <string_view>

#include "../border_colormaps/KinGroupIDBorderColorMap.hpp"
#include "../fill_colormaps/BooleanColorMap.hpp"
#include "../fill_colormaps/IsAliveColorMap.hpp"
#include "../getters/HeirRequestGetter.hpp"
#include "../getters/IsAliveGetter.hpp"
#include "../getters/KinGroupIDGetter.hpp"
#include "../renderers/CardinalFillRenderer.hpp"
#include "../renderers/CellBorderRenderer.hpp"
#include "../renderers/CellFillRenderer.hpp"

#include "Artist.hpp"

namespace dish2 {

namespace internal::heir_request_artist {

  template<
    typename HeirRequestGetter,
    typename IsAliveGetter,
    typename KinGroupIDGetter
  >
  using parent_t = dish2::Artist<
    dish2::CardinalFillRenderer<
      dish2::BooleanColorMap,
      HeirRequestGetter
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

} // namespace internal::heir_request_artist

template<
  typename Spec,
  typename HeirRequestGetter=dish2::HeirRequestGetter<Spec>,
  typename IsAliveGetter=dish2::IsAliveGetter<Spec>,
  typename KinGroupIDGetter=dish2::KinGroupIDGetter<Spec>
>
class HeirRequestArtist
: public internal::heir_request_artist::parent_t<
  HeirRequestGetter,
  IsAliveGetter,
  KinGroupIDGetter
> {

  using parent_t = internal::heir_request_artist::parent_t<
    HeirRequestGetter,
    IsAliveGetter,
    KinGroupIDGetter
  >;

public:

  // inherit constructors
  using parent_t::parent_t;

  static constexpr std::string_view GetName() { return "Heir Request"; }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_ARTISTS_HEIRREQUESTARTIST_HPP_INCLUDE
