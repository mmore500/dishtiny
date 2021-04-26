#pragma once
#ifndef DISH2_VIZ_ARTISTS_ISALIVEARTIST_HPP_INCLUDE
#define DISH2_VIZ_ARTISTS_ISALIVEARTIST_HPP_INCLUDE

#include <string_view>

#include "../border_colormaps/DummyBorderColorMap.hpp"
#include "../fill_colormaps/IsAliveColorMap.hpp"
#include "../getters/IsAliveGetter.hpp"
#include "../renderers/CellBorderRenderer.hpp"
#include "../renderers/CellFillRenderer.hpp"

#include "Artist.hpp"

namespace dish2 {

namespace internal::is_alive_artist {

  template<typename IsAliveGetter>
  using parent_t = dish2::Artist<
    dish2::CellFillRenderer<
      dish2::IsAliveColorMap,
      IsAliveGetter
    >,
    dish2::CellBorderRenderer<
      dish2::DummyBorderColorMap<>,
      IsAliveGetter
    >
  >;

} // namespace internal::is_alive_artist

template<
  typename Spec,
  typename IsAliveGetter=dish2::IsAliveGetter<Spec>
>
class IsAliveArtist
: public internal::is_alive_artist::parent_t<IsAliveGetter> {

  using parent_t = internal::is_alive_artist::parent_t<IsAliveGetter>;

public:

  // inherit constructors
  using parent_t::parent_t;

  static constexpr std::string_view GetName() { return "Alive"; }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_ARTISTS_ISALIVEARTIST_HPP_INCLUDE
