#pragma once
#ifndef DISH2_VIZ_ARTISTS_RESOURCEINPUTPEEKARTIST_HPP_INCLUDE
#define DISH2_VIZ_ARTISTS_RESOURCEINPUTPEEKARTIST_HPP_INCLUDE

#include <string_view>

#include "../border_colormaps/KinGroupIDBorderColorMap.hpp"
#include "../fill_colormaps/IsAliveColorMap.hpp"
#include "../fill_colormaps/ResourceInputPeekColorMap.hpp"
#include "../getters/IsAliveGetter.hpp"
#include "../getters/KinGroupIDGetter.hpp"
#include "../getters/ResourceInputPeekGetter.hpp"
#include "../renderers/CardinalFillRenderer.hpp"
#include "../renderers/CellBorderRenderer.hpp"
#include "../renderers/CellFillRenderer.hpp"

#include "Artist.hpp"

namespace dish2 {

namespace internal::resource_input_peek_artist {

  template<
    typename IsAliveGetter,
    typename KinGroupIDGetter,
    typename ResourceInputPeekGetter
  >
  using parent_t = dish2::Artist<
    dish2::CardinalFillRenderer<
      dish2::ResourceInputPeekColorMap,
      ResourceInputPeekGetter
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

} // namespace internal::resource_input_peek_artist

template<
  typename Spec,
  typename IsAliveGetter=dish2::IsAliveGetter<Spec>,
  typename KinGroupIDGetter=dish2::KinGroupIDGetter<Spec>,
  typename ResourceInputPeekGetter=dish2::ResourceInputPeekGetter<Spec>
>
class ResourceInputPeekArtist
: public internal::resource_input_peek_artist::parent_t<
  IsAliveGetter,
  KinGroupIDGetter,
  ResourceInputPeekGetter
> {

  using parent_t = internal::resource_input_peek_artist::parent_t<
    IsAliveGetter,
    KinGroupIDGetter,
    ResourceInputPeekGetter
    >;

public:

  // inherit constructors
  using parent_t::parent_t;

  static constexpr std::string_view GetName() { return "Received Resource"; }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_ARTISTS_RESOURCEINPUTPEEKARTIST_HPP_INCLUDE
