#pragma once
#ifndef DISH2_VIZ_ARTISTS_TAXAARTIST_HPP_INCLUDE
#define DISH2_VIZ_ARTISTS_TAXAARTIST_HPP_INCLUDE

#include <string_view>

#include "../border_colormaps/TaxaBorderColorMap.hpp"
#include "../fill_colormaps/IsAliveColorMap.hpp"
#include "../fill_colormaps/KinGroupIDGrayscaleFillColorMap.hpp"
#include "../getters/GenomeGetter.hpp"
#include "../getters/IsAliveGetter.hpp"
#include "../getters/KinGroupIDGetter.hpp"
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
  typename Spec,
  typename GenomeGetter=dish2::GenomeGetter<Spec>,
  typename IsAliveGetter=dish2::IsAliveGetter<Spec>,
  typename KinGroupIDGetter=dish2::KinGroupIDGetter<Spec>
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

  static constexpr std::string_view GetName() { return "Taxa"; }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_ARTISTS_TAXAARTIST_HPP_INCLUDE
