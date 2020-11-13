#pragma once
#ifndef DISH2_VIZ_ARTISTS_REGULATIONPROTECTEDBYMODULEARTIST_HPP_INCLUDE
#define DISH2_VIZ_ARTISTS_REGULATIONPROTECTEDBYMODULEARTIST_HPP_INCLUDE

#include <string>

#include "../../spec/Spec.hpp"

#include "../border_colormaps/KinGroupIDBorderColorMap.hpp"
#include "../fill_colormaps/IsAliveColorMap.hpp"
#include "../fill_colormaps/RegulationProtectedColorMap.hpp"
#include "../getters/IsAliveGetter.hpp"
#include "../getters/KinGroupIDGetter.hpp"
#include "../getters/RegulationProtectedByModuleGetter.hpp"
#include "../renderers/CellBorderRenderer.hpp"
#include "../renderers/CardinalFillRenderer.hpp"
#include "../renderers/CellFillRenderer.hpp"

#include "Artist.hpp"

namespace dish2 {

namespace internal::regulation_protected_by_module_artist {

  template<
    typename RegulationProtectedByModuleGetter,
    typename IsAliveGetter,
    typename KinGroupIDGetter
  >
  using parent_t = dish2::Artist<
    dish2::CardinalFillRenderer<
      dish2::RegulationProtectedColorMap,
      RegulationProtectedByModuleGetter
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

} // namespace internal::regulation_protected_by_module_artist

template<
  typename RegulationProtectedByModuleGetter
    =dish2::RegulationProtectedByModuleGetter<dish2::Spec>,
  typename IsAliveGetter=dish2::IsAliveGetter<dish2::Spec>,
  typename KinGroupIDGetter=dish2::KinGroupIDGetter<dish2::Spec>
>
class RegulationProtectedByModuleArtist
: public internal::regulation_protected_by_module_artist::parent_t<
  RegulationProtectedByModuleGetter,
  IsAliveGetter,
  KinGroupIDGetter
> {

  using parent_t = internal::regulation_protected_by_module_artist::parent_t<
    RegulationProtectedByModuleGetter,
    IsAliveGetter,
    KinGroupIDGetter
  >;

public:

  // inherit constructors
  using parent_t::parent_t;

  static std::string GetName() { return "Regulation Protected by Module"; }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_ARTISTS_REGULATIONPROTECTEDBYMODULEARTIST_HPP_INCLUDE
