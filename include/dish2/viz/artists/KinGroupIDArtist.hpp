#pragma once
#ifndef DISH2_VIZ_ARTISTS_KINGROUPIDARTIST_HPP_INCLUDE
#define DISH2_VIZ_ARTISTS_KINGROUPIDARTIST_HPP_INCLUDE

#include "../../spec/Spec.hpp"

#include "../border_colormaps/KinGroupIDBorderColorMap.hpp"
#include "../fill_colormaps/KinGroupIDFillColorMap.hpp"
#include "../getters/KinGroupIDGetter.hpp"
#include "../renderers/CellBorderRenderer.hpp"
#include "../renderers/CellFillRenderer.hpp"

#include "Artist.hpp"

namespace dish2 {

namespace internal::kin_group_id_artist {

  template<typename KinGroupIDGetter>
  using parent_t = dish2::Artist<
    dish2::CellFillRenderer<
      dish2::KinGroupIDFillColorMap,
      KinGroupIDGetter
    >,
    dish2::CellBorderRenderer<
      dish2::KinGroupIDBorderColorMap,
      KinGroupIDGetter
    >
  >;

} // namespace internal::kin_group_id_artist

template<typename KinGroupIDGetter=dish2::KinGroupIDGetter<dish2::Spec>>
class KinGroupIDArtist
: public internal::kin_group_id_artist::parent_t<KinGroupIDGetter> {

  using parent_t = internal::kin_group_id_artist::parent_t<KinGroupIDGetter>;

public:

  // inherit constructors
  using parent_t::parent_t;

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_ARTISTS_KINGROUPIDARTIST_HPP_INCLUDE
