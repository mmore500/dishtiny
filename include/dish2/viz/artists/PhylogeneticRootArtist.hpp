#pragma once
#ifndef DISH2_VIZ_ARTISTS_PHYLOGENETICROOTARTIST_HPP_INCLUDE
#define DISH2_VIZ_ARTISTS_PHYLOGENETICROOTARTIST_HPP_INCLUDE

#include <string_view>

#include "../border_colormaps/KinGroupIDBorderColorMap.hpp"
#include "../fill_colormaps/IsAliveColorMap.hpp"
#include "../fill_colormaps/PhylogeneticRootColorMap.hpp"
#include "../getters/IsAliveGetter.hpp"
#include "../getters/KinGroupIDGetter.hpp"
#include "../getters/PhylogeneticRootGetter.hpp"
#include "../renderers/CellBorderRenderer.hpp"
#include "../renderers/CellFillRenderer.hpp"

#include "Artist.hpp"

namespace dish2 {

namespace internal::phylogenetic_root_artist {

  template<
    typename PhylogeneticRootGetter,
    typename IsAliveGetter,
    typename KinGroupIDGetter
  >
  using parent_t = dish2::Artist<
    dish2::CellFillRenderer<
      dish2::PhylogeneticRootColorMap,
      PhylogeneticRootGetter
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

} // namespace internal::phylogenetic_root_artist

template<
  typename Spec,
  typename PhylogeneticRootGetter=dish2::PhylogeneticRootGetter<Spec>,
  typename IsAliveGetter=dish2::IsAliveGetter<Spec>,
  typename KinGroupIDGetter=dish2::KinGroupIDGetter<Spec>
>
class PhylogeneticRootArtist
: public internal::phylogenetic_root_artist::parent_t<
  PhylogeneticRootGetter,
  IsAliveGetter,
  KinGroupIDGetter
> {

  using parent_t = internal::phylogenetic_root_artist::parent_t<
    PhylogeneticRootGetter,
    IsAliveGetter,
    KinGroupIDGetter
  >;

public:

  // inherit constructors
  using parent_t::parent_t;

  static constexpr std::string_view GetName() { return "Phylogenetic Root"; }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_ARTISTS_PHYLOGENETICROOTARTIST_HPP_INCLUDE
