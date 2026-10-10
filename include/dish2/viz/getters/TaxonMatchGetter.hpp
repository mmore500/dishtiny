#pragma once
#ifndef DISH2_VIZ_GETTERS_TAXONMATCHGETTER_HPP_INCLUDE
#define DISH2_VIZ_GETTERS_TAXONMATCHGETTER_HPP_INCLUDE

#include <cstddef>
#include <string_view>
#include <tuple>

#include "../../world/ThreadWorld.hpp"

#include "GenomeGetter.hpp"
#include "NeighborPosGetter.hpp"

namespace dish2 {

template<typename Spec>
class TaxonMatchGetter {

  dish2::GenomeGetter<Spec> genome_getter;
  dish2::NeighborPosGetter<Spec> np_getter;

public:

  static constexpr std::string_view GetName() { return "TaxonMatch"; }

  using value_type = bool;

  template< typename... Args >
  TaxonMatchGetter( const dish2::ThreadWorld<Spec>& tw, Args&&... )
  : genome_getter( tw )
  , np_getter( tw )
  {}

  value_type Get( const size_t cell_idx, const size_t cardinal_idx=0 ) const {
    const auto focal_genome = genome_getter.Get( cell_idx );
    const auto neighbor_genome = genome_getter.Get(
      np_getter.Get( cell_idx, cardinal_idx )
    );

    return std::tuple{
      focal_genome.event_tags,
      focal_genome.program
    } == std::tuple{
      neighbor_genome.event_tags,
      neighbor_genome.program
    };
  }

  size_t GetNumCells() const { return np_getter.GetNumCells(); }

  size_t GetNumCardinals( const size_t cell_idx=0 ) const {
    return np_getter.GetNumCardinals( cell_idx );
  }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_GETTERS_TAXONMATCHGETTER_HPP_INCLUDE
