#pragma once
#ifndef DISH2_VIZ_ARTISTS_CARDINALORDERARTIST_HPP_INCLUDE
#define DISH2_VIZ_ARTISTS_CARDINALORDERARTIST_HPP_INCLUDE

#include <string>

#include "../../spec/Spec.hpp"

#include "../border_colormaps/KinGroupIDBorderColorMap.hpp"
#include "../fill_colormaps/PhylogeneticRootColorMap.hpp"
#include "../getters/CardinalOrderGetter.hpp"
#include "../getters/KinGroupIDGetter.hpp"
#include "../renderers/CellBorderRenderer.hpp"
#include "../renderers/CellFillRenderer.hpp"
#include "../renderers/CardinalFillRenderer.hpp"

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
  typename CardinalOrderGetter=dish2::CardinalOrderGetter<dish2::Spec>,
  typename KinGroupIDGetter=dish2::KinGroupIDGetter<dish2::Spec>
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

  static std::string GetName() { return "Cardinal Order"; }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_ARTISTS_CARDINALORDERARTIST_HPP_INCLUDE
