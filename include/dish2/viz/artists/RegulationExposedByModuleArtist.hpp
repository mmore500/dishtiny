#pragma once
#ifndef DISH2_VIZ_ARTISTS_REGULATIONEXPOSEDBYMODULEARTIST_HPP_INCLUDE
#define DISH2_VIZ_ARTISTS_REGULATIONEXPOSEDBYMODULEARTIST_HPP_INCLUDE

#include <algorithm>
#include <string>

#include "../../introspection/get_maximum_module_count.hpp"
#include "../../spec/Spec.hpp"

#include "../border_colormaps/KinGroupIDBorderColorMap.hpp"
#include "../fill_colormaps/IsAliveColorMap.hpp"
#include "../fill_colormaps/RegulationExposedColorMap.hpp"
#include "../getters/IsAliveGetter.hpp"
#include "../getters/KinGroupIDGetter.hpp"
#include "../getters/RegulationExposedByModuleGetter.hpp"
#include "../renderers/CardinalFillRenderer.hpp"
#include "../renderers/CellBorderRenderer.hpp"
#include "../renderers/CellFillRenderer.hpp"

#include "Artist.hpp"

namespace dish2 {

namespace internal::regulation_exposed_by_module_artist {

  template<
    typename RegulationExposedByModuleGetter,
    typename IsAliveGetter,
    typename KinGroupIDGetter
  >
  using parent_t = dish2::Artist<
    dish2::CardinalFillRenderer<
      dish2::RegulationExposedColorMap,
      RegulationExposedByModuleGetter
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

} // namespace internal::regulation_exposed_by_module_artist

template<
  typename RegulationExposedByModuleGetter
    =dish2::RegulationExposedByModuleGetter<dish2::Spec>,
  typename IsAliveGetter=dish2::IsAliveGetter<dish2::Spec>,
  typename KinGroupIDGetter=dish2::KinGroupIDGetter<dish2::Spec>
>
class RegulationExposedByModuleArtist
: public internal::regulation_exposed_by_module_artist::parent_t<
  RegulationExposedByModuleGetter,
  IsAliveGetter,
  KinGroupIDGetter
> {

  using parent_t = internal::regulation_exposed_by_module_artist::parent_t<
    RegulationExposedByModuleGetter,
    IsAliveGetter,
    KinGroupIDGetter
  >;

public:

  // inherit constructors
  using parent_t::parent_t;

  static std::string GetName() { return "Regulation Exposed by Module"; }

  template< typename Spec >
  static size_t GetSeriesLength(const dish2::ThreadWorld<Spec>& thread_world) {
    return std::min(
      32ul,
      dish2::get_maximum_module_count<Spec>( thread_world )
    );
  }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_ARTISTS_REGULATIONEXPOSEDBYMODULEARTIST_HPP_INCLUDE
