#pragma once
#ifndef DISH2_VIZ_ARTISTS_KINGROUPIDLEVARTIST_HPP_INCLUDE
#define DISH2_VIZ_ARTISTS_KINGROUPIDLEVARTIST_HPP_INCLUDE

#include <string_view>

#include "../../spec/Spec.hpp"

#include "../border_colormaps/MatchBorderColorMap.hpp"
#include "../fill_colormaps/IsAliveColorMap.hpp"
#include "../fill_colormaps/KinGroupIDLevFillColorMap.hpp"
#include "../getters/IsAliveGetter.hpp"
#include "../getters/KinGroupIDLevGetter.hpp"
#include "../renderers/CellBorderRenderer.hpp"
#include "../renderers/CellFillRenderer.hpp"

#include "Artist.hpp"

namespace dish2 {

namespace internal::kin_group_id_lev_artist {

  template<
    typename IsAliveGetter,
    typename KinGroupIDLevGetter
  >
  using parent_t = dish2::Artist<
    dish2::CellFillRenderer<
      dish2::KinGroupIDLevFillColorMap,
      KinGroupIDLevGetter
    >,
    dish2::CellFillRenderer<
      dish2::IsAliveColorMap,
      IsAliveGetter
    >,
    dish2::CellBorderRenderer<
      dish2::MatchBorderColorMap,
      KinGroupIDLevGetter
    >
  >;

} // namespace internal::kin_group_id_lev_artist

template<
  typename IsAliveGetter=dish2::IsAliveGetter<dish2::Spec>,
  typename KinGroupIDLevGetter=dish2::KinGroupIDLevGetter<dish2::Spec>
>
class KinGroupIDLevArtist
: public internal::kin_group_id_lev_artist::parent_t<
  IsAliveGetter,
  KinGroupIDLevGetter
> {

  using parent_t = internal::kin_group_id_lev_artist::parent_t<
    IsAliveGetter,
    KinGroupIDLevGetter
  >;

public:

  // inherit constructors
  using parent_t::parent_t;

  static constexpr std::string_view GetName() { return "Kin Group ID by Lev"; }

  template< typename Spec >
  static size_t GetSeriesLength(const dish2::ThreadWorld<Spec>& thread_world) {
    return Spec::NLEV;
  }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_ARTISTS_KINGROUPIDLEVARTIST_HPP_INCLUDE
