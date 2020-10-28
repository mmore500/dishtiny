#pragma once
#ifndef DISH2_VIZ_GETTERS_KINGROUPIDGETTER_HPP_INCLUDE
#define DISH2_VIZ_GETTERS_KINGROUPIDGETTER_HPP_INCLUDE

#include <functional>

#include "../../cell/Cell.hpp"
#include "../../config/cfg.hpp"
#include "../../genome/Genome.hpp"
#include "../../genome/KinGroupID.hpp"
#include "../../world/ThreadWorld.hpp"

namespace dish2 {

template<typename Spec>
class KinGroupIDGetter {

  std::reference_wrapper<const dish2::ThreadWorld<Spec>> thread_world;

public:

  using value_type = dish2::KinGroupID<Spec>;

  KinGroupIDGetter(const dish2::ThreadWorld<Spec>& thread_world_)
  : thread_world(thread_world_)
  {}

  const value_type Get(
    const size_t cell_idx, const size_t cardinal_idx=0
  ) const {
    return thread_world.get().GetCell( cell_idx ).GetGenome().kin_group_id;
  }

  size_t GetNumCells() const { return thread_world.get().GetSize(); }

  size_t GetNumCardinals( const size_t cell_idx=0 ) const {
    return thread_world.get().GetCell( cell_idx ).GetNumCardinals();
  }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_GETTERS_KINGROUPIDGETTER_HPP_INCLUDE
