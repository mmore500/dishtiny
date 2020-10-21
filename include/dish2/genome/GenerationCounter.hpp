#pragma once
#ifndef DISH2_GENOME_GENERATIONCOUNTER_HPP_INCLUDE
#define DISH2_GENOME_GENERATIONCOUNTER_HPP_INCLUDE

#include <algorithm>
#include <cstddef>

namespace dish2 {

template<typename Spec>
struct GenerationCounter {

  emp::array< size_t, Spec::NLEV + 1 > elapsed_generations{};

  void ElapseGeneration( const size_t lev ) {
    std::transform(
      std::begin( elapsed_generations ),
      std::next( std::begin( elapsed_generations ), lev + 1 ),
      std::begin( elapsed_generations ),
      []( const size_t cur ){ return cur + 1; }
    );
  }

  bool operator==(const GenerationCounter& other) const {
    return elapsed_generations == other.elapsed_generations;
  }

  template<typename Archive> void serialize( Archive & ar ) { ar(
    elapsed_generations
  ); }

};


} // namespace dish2

#endif // #ifndef DISH2_GENOME_GENERATIONCOUNTER_HPP_INCLUDE
