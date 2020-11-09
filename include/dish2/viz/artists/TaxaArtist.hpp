#pragma once
#ifndef DISH2_VIZ_ARTISTS_TAXAARTIST_HPP_INCLUDE
#define DISH2_VIZ_ARTISTS_TAXAARTIST_HPP_INCLUDE

#include <string>

#include "../../spec/Spec.hpp"

#include "../border_colormaps/TaxaBorderColorMap.hpp"
#include "../fill_colormaps/IsAliveColorMap.hpp"
#include "../fill_colormaps/KinGroupIDGrayscaleFillColorMap.hpp"
#include "../getters/IsAliveGetter.hpp"
#include "../getters/KinGroupIDGetter.hpp"
#include "../getters/GenomeGetter.hpp"
#include "../renderers/CellBorderRenderer.hpp"
#include "../renderers/CellFillRenderer.hpp"

#include "Artist.hpp"

namespace dish2 {

namespace internal::taxa_artist {

  template<
    typename GenomeGetter,
    typename IsAliveGetter,
    typename KinGroupIDGetter
  >
  using parent_t = dish2::Artist<
    dish2::CellFillRenderer<
      dish2::IsAliveColorMap,
      IsAliveGetter
    >,
    dish2::CellBorderRenderer<
      dish2::TaxaBorderColorMap,
      GenomeGetter
    >
  >;

} // namespace internal::taxa_artist

template<
  typename GenomeGetter=dish2::GenomeGetter<dish2::Spec>,
  typename IsAliveGetter=dish2::IsAliveGetter<dish2::Spec>,
  typename KinGroupIDGetter=dish2::KinGroupIDGetter<dish2::Spec>
>
class TaxaArtist
: public internal::taxa_artist::parent_t<
  GenomeGetter,
  IsAliveGetter,
  KinGroupIDGetter
> {

  using parent_t = internal::taxa_artist::parent_t<
    GenomeGetter,
    IsAliveGetter,
    KinGroupIDGetter
  >;

public:

  // inherit constructors
  using parent_t::parent_t;

  static std::string GetName() { return "Taxa"; }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_ARTISTS_TAXAARTIST_HPP_INCLUDE
