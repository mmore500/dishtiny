#pragma once
#ifndef DISH2_WEB_ARTISTS_ISALIVEARTIST_HPP_INCLUDE
#define DISH2_WEB_ARTISTS_ISALIVEARTIST_HPP_INCLUDE

#include "../border_colormaps/DummyBorderColorMap.hpp"
#include "../fill_colormaps/IsAliveColorMap.hpp"
#include "../renderers/CellBorderRenderer.hpp"
#include "../renderers/CellFillRenderer.hpp"

#include "Artist.hpp"

namespace dish2 {

namespace internal::resource_stockpile_artist {

  template<typename Getter>
  using parent_t = dish2::Artist<
    dish2::CellFillRenderer<
      dish2::IsAliveColorMap,
      Getter
    >,
    dish2::CellBorderRenderer<
      dish2::DummyBorderColorMap<>,
      Getter
    >
  >;

} // namespace internal::resource_stockpile_artist

template<typename Getter>
class IsAliveArtist
: public internal::resource_stockpile_artist::parent_t<Getter> {

  using parent_t = internal::resource_stockpile_artist::parent_t<Getter>;

public:

  // inherit constructors
  using parent_t::parent_t;

};

} // namespace dish2

#endif // #ifndef DISH2_WEB_ARTISTS_ISALIVEARTIST_HPP_INCLUDE
