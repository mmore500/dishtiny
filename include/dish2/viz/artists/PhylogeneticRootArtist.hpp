#pragma once
#ifndef DISH2_WEB_ARTISTS_PHYLOGENETICROOTARTIST_HPP_INCLUDE
#define DISH2_WEB_ARTISTS_PHYLOGENETICROOTARTIST_HPP_INCLUDE

#include "../border_colormaps/DummyBorderColorMap.hpp"
#include "../fill_colormaps/PhylogeneticRootColorMap.hpp"
#include "../renderers/CellBorderRenderer.hpp"
#include "../renderers/CellFillRenderer.hpp"

#include "Artist.hpp"

namespace dish2 {

namespace internal::phylogenetic_root_artist {

  template<typename Getter>
  using parent_t = dish2::Artist<
    dish2::CellFillRenderer<
      dish2::PhylogeneticRootColorMap,
      Getter
    >,
    dish2::CellBorderRenderer<
      dish2::DummyBorderColorMap<>,
      Getter
    >
  >;

} // namespace internal::phylogenetic_root_artist

template<typename Getter>
class PhylogeneticRootArtist
: public internal::phylogenetic_root_artist::parent_t<Getter> {

  using parent_t = internal::phylogenetic_root_artist::parent_t<Getter>;

public:

  // inherit constructors
  using parent_t::parent_t;

};

} // namespace dish2

#endif // #ifndef DISH2_WEB_ARTISTS_PHYLOGENETICROOTARTIST_HPP_INCLUDE
