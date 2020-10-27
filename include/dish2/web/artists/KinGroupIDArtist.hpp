#pragma once
#ifndef DISH2_WEB_ARTISTS_KINGROUPIDARTIST_HPP_INCLUDE
#define DISH2_WEB_ARTISTS_KINGROUPIDARTIST_HPP_INCLUDE

#include "../border_colormaps/KinGroupIDBorderColorMap.hpp"
#include "../fill_colormaps/KinGroupIDFillColorMap.hpp"
#include "../renderers/CellBorderRenderer.hpp"
#include "../renderers/CellFillRenderer.hpp"

#include "Artist.hpp"

namespace dish2 {

namespace internal::kin_group_id_artist {

  template<typename Getter>
  using parent_t = dish2::Artist<
    dish2::CellFillRenderer<
      dish2::KinGroupIDFillColorMap,
      Getter
    >,
    dish2::CellBorderRenderer<
      dish2::KinGroupIDBorderColorMap,
      Getter
    >
  >;

} // namespace internal::kin_group_id_artist

template<typename Getter>
class KinGroupIDArtist
: public internal::kin_group_id_artist::parent_t<Getter> {

  using parent_t = internal::kin_group_id_artist::parent_t<Getter>;

public:

  // inherit constructors
  using parent_t::parent_t;

};

} // namespace dish2

#endif // #ifndef DISH2_WEB_ARTISTS_KINGROUPIDARTIST_HPP_INCLUDE
