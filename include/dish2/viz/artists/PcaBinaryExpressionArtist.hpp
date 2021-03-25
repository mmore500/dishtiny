#pragma once
#ifndef DISH2_VIZ_ARTISTS_PCABINARYEXPRESSIONARTIST_HPP_INCLUDE
#define DISH2_VIZ_ARTISTS_PCABINARYEXPRESSIONARTIST_HPP_INCLUDE

#include <string_view>

#include "../../spec/Spec.hpp"

#include "../border_colormaps/KinGroupIDBorderInvertColorMap.hpp"
#include "../fill_colormaps/IsAliveColorMap.hpp"
#include "../fill_colormaps/PcaBinaryExpressionColorMap.hpp"
#include "../getters/CardiCoordGetter.hpp"
#include "../getters/IsAliveGetter.hpp"
#include "../getters/KinGroupIDGetter.hpp"
#include "../renderers/CardinalFillRenderer.hpp"
#include "../renderers/CellBorderRenderer.hpp"
#include "../renderers/CellFillRenderer.hpp"

#include "Artist.hpp"

namespace dish2 {

namespace internal::pca_binary_expression_artist {

  template<
    typename CardiCoordGetter,
    typename IsAliveGetter,
    typename KinGroupIDGetter
  >
  using parent_t = dish2::Artist<
    dish2::CardinalFillRenderer<
      dish2::PcaBinaryExpressionColorMap,
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

} // namespace internal::pca_binary_expression_artist

template<
  typename CardiCoordGetter=dish2::CardiCoordGetter<dish2::Spec>,
  typename IsAliveGetter=dish2::IsAliveGetter<dish2::Spec>,
  typename KinGroupIDGetter=dish2::KinGroupIDGetter<dish2::Spec>
>
class PcaBinaryExpressionArtist
: public internal::pca_binary_expression_artist::parent_t<
  CardiCoordGetter,
  IsAliveGetter,
  KinGroupIDGetter
> {

  using parent_t = internal::pca_binary_expression_artist::parent_t<
    CardiCoordGetter,
    IsAliveGetter,
    KinGroupIDGetter
  >;

public:

  // inherit constructors
  using parent_t::parent_t;

  static constexpr std::string_view GetName() { return "Binary Module Expression"; }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_ARTISTS_PCABINARYEXPRESSIONARTIST_HPP_INCLUDE
