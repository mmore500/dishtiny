#pragma once
#ifndef DISH2_VIZ_ARTISTS_NUMBUSYCORESARTIST_HPP_INCLUDE
#define DISH2_VIZ_ARTISTS_NUMBUSYCORESARTIST_HPP_INCLUDE

#include <string_view>

#include "../border_colormaps/KinGroupIDBorderColorMap.hpp"
#include "../fill_colormaps/IsAliveColorMap.hpp"
#include "../fill_colormaps/NumBusyCoresColorMap.hpp"
#include "../getters/IsAliveGetter.hpp"
#include "../getters/KinGroupIDGetter.hpp"
#include "../getters/NumBusyCoresGetter.hpp"
#include "../renderers/CardinalFillRenderer.hpp"
#include "../renderers/CellBorderRenderer.hpp"
#include "../renderers/CellFillRenderer.hpp"

#include "Artist.hpp"

namespace dish2 {

namespace internal::num_busy_cores_artist {

  template<
    typename NumBusyCoresGetter,
    typename IsAliveGetter,
    typename KinGroupIDGetter
  >
  using parent_t = dish2::Artist<
    dish2::CardinalFillRenderer<
      dish2::NumBusyCoresColorMap,
      NumBusyCoresGetter
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

} // namespace internal::num_busy_cores_artist

template<
  typename Spec,
  typename NumBusyCoresGetter=dish2::NumBusyCoresGetter<Spec>,
  typename IsAliveGetter=dish2::IsAliveGetter<Spec>,
  typename KinGroupIDGetter=dish2::KinGroupIDGetter<Spec>
>
class NumBusyCoresArtist
: public internal::num_busy_cores_artist::parent_t<
  NumBusyCoresGetter,
  IsAliveGetter,
  KinGroupIDGetter
> {

  using parent_t = internal::num_busy_cores_artist::parent_t<
    NumBusyCoresGetter,
    IsAliveGetter,
    KinGroupIDGetter
  >;

public:

  // inherit constructors
  using parent_t::parent_t;

  static constexpr std::string_view GetName() { return "Num Busy Cores"; }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_ARTISTS_NUMBUSYCORESARTIST_HPP_INCLUDE
