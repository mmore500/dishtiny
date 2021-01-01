#pragma once
#ifndef DISH2_VIZ_ARTISTS_ISPERIPHERALLEVARTIST_HPP_INCLUDE
#define DISH2_VIZ_ARTISTS_ISPERIPHERALLEVARTIST_HPP_INCLUDE

#include <string>

#include "../../spec/Spec.hpp"

#include "../border_colormaps/MatchBorderColorMap.hpp"
#include "../fill_colormaps/IsAliveColorMap.hpp"
#include "../fill_colormaps/BooleanColorMap.hpp"
#include "../getters/IsPeripheralLevGetter.hpp"
#include "../getters/IsPeripheralLevGetter.hpp"
#include "../renderers/CellBorderRenderer.hpp"
#include "../renderers/CellFillRenderer.hpp"

#include "Artist.hpp"

namespace dish2 {

namespace internal::is_peripheral_lev_artist {

  template<
    typename IsAliveGetter,
    typename IsPeripheralLevGetter
  >
  using parent_t = dish2::Artist<
    dish2::CellFillRenderer<
      dish2::BooleanColorMap,
      IsPeripheralLevGetter
    >,
    dish2::CellFillRenderer<
      dish2::IsAliveColorMap,
      IsAliveGetter
    >,
    dish2::CellBorderRenderer<
      dish2::MatchBorderColorMap,
      IsPeripheralLevGetter
    >
  >;

} // namespace internal::is_peripheral_lev_artist

template<
  typename IsAliveGetter=dish2::IsAliveGetter<dish2::Spec>,
  typename IsPeripheralLevGetter=dish2::IsPeripheralLevGetter<dish2::Spec>
>
class IsPeripheralLevArtist
: public internal::is_peripheral_lev_artist::parent_t<
  IsAliveGetter,
  IsPeripheralLevGetter
> {

  using parent_t = internal::is_peripheral_lev_artist::parent_t<
    IsAliveGetter,
    IsPeripheralLevGetter
  >;

public:

  // inherit constructors
  using parent_t::parent_t;

  static std::string GetName() { return "Is Peripheral by Lev"; }

  template< typename Spec >
  static size_t GetSeriesLength(const dish2::ThreadWorld<Spec>& thread_world) {
    return Spec::NLEV;
  }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_ARTISTS_ISPERIPHERALLEVARTIST_HPP_INCLUDE
