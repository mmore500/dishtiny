#pragma once
#ifndef DISH2_VIZ_GETTERS_LEARNEDQUORUMBITSGETTER_HPP_INCLUDE
#define DISH2_VIZ_GETTERS_LEARNEDQUORUMBITSGETTER_HPP_INCLUDE

#include <functional>

#include "../../cell/Cell.hpp"
#include "../../config/cfg.hpp"
#include "../../genome/Genome.hpp"
#include "../../genome/RootID.hpp"
#include "../../world/ThreadWorld.hpp"

namespace dish2 {

template<typename Spec>
class LearnedQuorumBitsGetter {

  std::reference_wrapper<const dish2::ThreadWorld<Spec>> thread_world;

  size_t lev;

public:

  using value_type = size_t;

  template< typename... Args >
  LearnedQuorumBitsGetter(
    const dish2::ThreadWorld<Spec>& thread_world_,
    const size_t idx,
    Args&&...
  )
  : thread_world(thread_world_)
  , lev( idx )
  {}

  const value_type Get(
    const size_t cell_idx, const size_t cardinal_idx=0
  ) const {
    return thread_world.get().GetCell(
      cell_idx
    ).cardinals[ cardinal_idx ].cardinal_quorum_state.GetNumLearnedBits(
      lev
    );
  }

  size_t GetNumCells() const { return thread_world.get().GetSize(); }

  size_t GetNumCardinals( const size_t cell_idx=0 ) const {
    return thread_world.get().GetCell( cell_idx ).GetNumCardinals();
  }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_GETTERS_LEARNEDQUORUMBITSGETTER_HPP_INCLUDE
