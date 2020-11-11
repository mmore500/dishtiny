#pragma once
#ifndef DISH2_VIZ_ARTISTS_REGULATIONBYMODULEARTIST_HPP_INCLUDE
#define DISH2_VIZ_ARTISTS_REGULATIONBYMODULEARTIST_HPP_INCLUDE

#include <string>

#include "../../spec/Spec.hpp"

#include "../border_colormaps/KinGroupIDBorderColorMap.hpp"
#include "../fill_colormaps/IsAliveColorMap.hpp"
#include "../fill_colormaps/RegulationColorMap.hpp"
#include "../getters/IsAliveGetter.hpp"
#include "../getters/KinGroupIDGetter.hpp"
#include "../getters/RegulationByModuleGetter.hpp"
#include "../renderers/CellBorderRenderer.hpp"
#include "../renderers/CardinalFillRenderer.hpp"
#include "../renderers/CellFillRenderer.hpp"

#include "Artist.hpp"

namespace dish2 {

namespace internal::regulation_by_module_artist {

  template<
    typename RegulationByModuleGetter,
    typename IsAliveGetter,
    typename KinGroupIDGetter
  >
  using parent_t = dish2::Artist<
    dish2::CardinalFillRenderer<
      dish2::RegulationColorMap,
      RegulationByModuleGetter
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

} // namespace internal::regulation_by_module_artist

template<
  typename RegulationByModuleGetter
    =dish2::RegulationByModuleGetter<dish2::Spec>,
  typename IsAliveGetter=dish2::IsAliveGetter<dish2::Spec>,
  typename KinGroupIDGetter=dish2::KinGroupIDGetter<dish2::Spec>
>
class RegulationByModuleArtist
: public internal::regulation_by_module_artist::parent_t<
  RegulationByModuleGetter,
  IsAliveGetter,
  KinGroupIDGetter
> {

  using parent_t = internal::regulation_by_module_artist::parent_t<
    RegulationByModuleGetter,
    IsAliveGetter,
    KinGroupIDGetter
  >;

public:

  // inherit constructors
  using parent_t::parent_t;

  static std::string GetName() { return "Regulation by Module"; }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_ARTISTS_REGULATIONBYMODULEARTIST_HPP_INCLUDE
