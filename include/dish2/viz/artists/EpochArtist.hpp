#pragma once
#ifndef DISH2_VIZ_ARTISTS_EPOCHARTIST_HPP_INCLUDE
#define DISH2_VIZ_ARTISTS_EPOCHARTIST_HPP_INCLUDE

#include <string_view>

#include "../border_colormaps/KinGroupIDBorderColorMap.hpp"
#include "../fill_colormaps/EpochColorMap.hpp"
#include "../fill_colormaps/IsAliveColorMap.hpp"
#include "../getters/EpochGetter.hpp"
#include "../getters/IsAliveGetter.hpp"
#include "../getters/KinGroupIDGetter.hpp"
#include "../renderers/CellBorderRenderer.hpp"
#include "../renderers/CellFillRenderer.hpp"

#include "Artist.hpp"

namespace dish2 {

namespace internal::epoch_artist {

  template<
    typename EpochGetter,
    typename IsAliveGetter,
    typename KinGroupIDGetter
  >
  using parent_t = dish2::Artist<
    dish2::CellFillRenderer<
      dish2::EpochColorMap,
      EpochGetter
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

} // namespace internal::epoch_artist

template<
  typename Spec,
  typename EpochGetter=dish2::EpochGetter<Spec>,
  typename IsAliveGetter=dish2::IsAliveGetter<Spec>,
  typename KinGroupIDGetter=dish2::KinGroupIDGetter<Spec>
>
class EpochArtist
: public internal::epoch_artist::parent_t<
  EpochGetter,
  IsAliveGetter,
  KinGroupIDGetter
> {

  using parent_t = internal::epoch_artist::parent_t<
    EpochGetter,
    IsAliveGetter,
    KinGroupIDGetter
  >;

public:

  // inherit constructors
  using parent_t::parent_t;

  static constexpr std::string_view GetName() { return "Epoch"; }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_ARTISTS_EPOCHARTIST_HPP_INCLUDE
