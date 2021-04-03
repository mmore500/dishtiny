#pragma once
#ifndef DISH2_VIZ_ARTISTS_CARDINALORDERARTIST_HPP_INCLUDE
#define DISH2_VIZ_ARTISTS_CARDINALORDERARTIST_HPP_INCLUDE

#include <string_view>

#include "../border_colormaps/KinGroupIDBorderColorMap.hpp"
#include "../fill_colormaps/PhylogeneticRootColorMap.hpp"
#include "../getters/CardinalOrderGetter.hpp"
#include "../getters/KinGroupIDGetter.hpp"
#include "../renderers/CardinalFillRenderer.hpp"
#include "../renderers/CellBorderRenderer.hpp"
#include "../renderers/CellFillRenderer.hpp"

#include "Artist.hpp"

namespace dish2 {

namespace internal::cardinal_order_artist {

  template<
    typename CardinalOrderGetter,
    typename KinGroupIDGetter
  >
  using parent_t = dish2::Artist<
    dish2::CardinalFillRenderer<
      dish2::PhylogeneticRootColorMap,
      CardinalOrderGetter
    >,
    dish2::CellBorderRenderer<
      dish2::KinGroupIDBorderColorMap,
      KinGroupIDGetter
    >
  >;

} // namespace internal::cardinal_order_artist

template<
  typename Spec,
  typename CardinalOrderGetter=dish2::CardinalOrderGetter<Spec>,
  typename KinGroupIDGetter=dish2::KinGroupIDGetter<Spec>
>
class CardinalOrderArtist
: public internal::cardinal_order_artist::parent_t<
  CardinalOrderGetter,
  KinGroupIDGetter
> {

  using parent_t = internal::cardinal_order_artist::parent_t<
    CardinalOrderGetter,
    KinGroupIDGetter
  >;

public:

  // inherit constructors
  using parent_t::parent_t;

  static constexpr std::string_view GetName() { return "Cardinal Order"; }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_ARTISTS_CARDINALORDERARTIST_HPP_INCLUDE
