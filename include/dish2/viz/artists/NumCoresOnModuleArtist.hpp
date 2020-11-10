#pragma once
#ifndef DISH2_VIZ_ARTISTS_NUMCORESONMODULEARTIST_HPP_INCLUDE
#define DISH2_VIZ_ARTISTS_NUMCORESONMODULEARTIST_HPP_INCLUDE

#include <string>

#include "../../spec/Spec.hpp"

#include "../border_colormaps/KinGroupIDBorderColorMap.hpp"
#include "../fill_colormaps/IsAliveColorMap.hpp"
#include "../fill_colormaps/NumBusyCoresColorMap.hpp"
#include "../getters/IsAliveGetter.hpp"
#include "../getters/KinGroupIDGetter.hpp"
#include "../getters/NumCoresOnModuleGetter.hpp"
#include "../renderers/CellBorderRenderer.hpp"
#include "../renderers/CardinalFillRenderer.hpp"
#include "../renderers/CellFillRenderer.hpp"

#include "Artist.hpp"

namespace dish2 {

namespace internal::num_cores_on_module_artist {

  template<
    typename NumCoresOnModuleGetter,
    typename IsAliveGetter,
    typename KinGroupIDGetter
  >
  using parent_t = dish2::Artist<
    dish2::CardinalFillRenderer<
      dish2::NumBusyCoresColorMap,
      NumCoresOnModuleGetter
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

} // namespace internal::num_cores_on_module_artist

template<
  typename NumCoresOnModuleGetter
    =dish2::NumCoresOnModuleGetter<dish2::Spec>,
  typename IsAliveGetter=dish2::IsAliveGetter<dish2::Spec>,
  typename KinGroupIDGetter=dish2::KinGroupIDGetter<dish2::Spec>
>
class NumCoresOnModuleArtist
: public internal::num_cores_on_module_artist::parent_t<
  NumCoresOnModuleGetter,
  IsAliveGetter,
  KinGroupIDGetter
> {

  using parent_t = internal::num_cores_on_module_artist::parent_t<
    NumCoresOnModuleGetter,
    IsAliveGetter,
    KinGroupIDGetter
  >;

public:

  // inherit constructors
  using parent_t::parent_t;

  static std::string GetName() { return "Num Cores On Module"; }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_ARTISTS_NUMCORESONMODULEARTIST_HPP_INCLUDE
