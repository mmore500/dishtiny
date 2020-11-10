#pragma once
#ifndef DISH2_VIZ_ARTISTS_NUMMODULESARTIST_HPP_INCLUDE
#define DISH2_VIZ_ARTISTS_NUMMODULESARTIST_HPP_INCLUDE

#include <string>

#include "../../spec/Spec.hpp"

#include "../border_colormaps/KinGroupIDBorderColorMap.hpp"
#include "../fill_colormaps/IsAliveColorMap.hpp"
#include "../fill_colormaps/NumBusyCoresColorMap.hpp"
#include "../getters/IsAliveGetter.hpp"
#include "../getters/KinGroupIDGetter.hpp"
#include "../getters/NumModulesGetter.hpp"
#include "../renderers/CellBorderRenderer.hpp"
#include "../renderers/CardinalFillRenderer.hpp"
#include "../renderers/CellFillRenderer.hpp"

#include "Artist.hpp"

namespace dish2 {

namespace internal::num_modules_artist {

  template<
    typename NumModulesGetter,
    typename IsAliveGetter,
    typename KinGroupIDGetter
  >
  using parent_t = dish2::Artist<
    dish2::CellFillRenderer<
      dish2::NumBusyCoresColorMap,
      NumModulesGetter
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

} // namespace internal::num_modules_artist

template<
  typename NumModulesGetter
    =dish2::NumModulesGetter<dish2::Spec>,
  typename IsAliveGetter=dish2::IsAliveGetter<dish2::Spec>,
  typename KinGroupIDGetter=dish2::KinGroupIDGetter<dish2::Spec>
>
class NumModulesArtist
: public internal::num_modules_artist::parent_t<
  NumModulesGetter,
  IsAliveGetter,
  KinGroupIDGetter
> {

  using parent_t = internal::num_modules_artist::parent_t<
    NumModulesGetter,
    IsAliveGetter,
    KinGroupIDGetter
  >;

public:

  // inherit constructors
  using parent_t::parent_t;

  static std::string GetName() { return "Num Program Modules"; }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_ARTISTS_NUMMODULESARTIST_HPP_INCLUDE
