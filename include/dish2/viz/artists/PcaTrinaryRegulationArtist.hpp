#pragma once
#ifndef DISH2_VIZ_ARTISTS_PCATRINARYREGULATIONARTIST_HPP_INCLUDE
#define DISH2_VIZ_ARTISTS_PCATRINARYREGULATIONARTIST_HPP_INCLUDE

#include <string_view>

#include "../border_colormaps/KinGroupIDBorderInvertColorMap.hpp"
#include "../fill_colormaps/IsAliveColorMap.hpp"
#include "../fill_colormaps/PcaTrinaryRegulationColorMap.hpp"
#include "../getters/CardiCoordGetter.hpp"
#include "../getters/IsAliveGetter.hpp"
#include "../getters/KinGroupIDGetter.hpp"
#include "../renderers/CardinalFillRenderer.hpp"
#include "../renderers/CellBorderRenderer.hpp"
#include "../renderers/CellFillRenderer.hpp"

#include "Artist.hpp"

namespace dish2 {

namespace internal::pca_trinary_regulation_artist {

  template<
    typename Spec,
    typename CardiCoordGetter,
    typename IsAliveGetter,
    typename KinGroupIDGetter
  >
  using parent_t = dish2::Artist<
    dish2::CardinalFillRenderer<
      dish2::PcaTrinaryRegulationColorMap<Spec>,
      CardiCoordGetter
    >,
    dish2::CellFillRenderer<
      dish2::IsAliveColorMap,
      IsAliveGetter
    >,
    dish2::CellBorderRenderer<
      dish2::KinGroupIDBorderInvertColorMap,
      KinGroupIDGetter
    >
  >;

} // namespace internal::pca_trinary_regulation_artist

template<
  typename Spec,
  typename CardiCoordGetter=dish2::CardiCoordGetter<Spec>,
  typename IsAliveGetter=dish2::IsAliveGetter<Spec>,
  typename KinGroupIDGetter=dish2::KinGroupIDGetter<Spec>
>
class PcaTrinaryRegulationArtist
: public internal::pca_trinary_regulation_artist::parent_t<
  Spec,
  CardiCoordGetter,
  IsAliveGetter,
  KinGroupIDGetter
> {

  using parent_t = internal::pca_trinary_regulation_artist::parent_t<
    Spec,
    CardiCoordGetter,
    IsAliveGetter,
    KinGroupIDGetter
  >;

public:

  // inherit constructors
  using parent_t::parent_t;

  static constexpr std::string_view GetName() { return "Trinary Module Regulation"; }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_ARTISTS_PCATRINARYREGULATIONARTIST_HPP_INCLUDE
