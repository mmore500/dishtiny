#pragma once
#ifndef DISH2_VIZ_GETTERS_CARDICOORDGETTER_HPP_INCLUDE
#define DISH2_VIZ_GETTERS_CARDICOORDGETTER_HPP_INCLUDE

#include <functional>
#include <tuple>

#include "../../cell/Cell.hpp"
#include "../../config/cfg.hpp"
#include "../../genome/Genome.hpp"
#include "../../genome/RootID.hpp"
#include "../../world/ThreadWorld.hpp"

namespace dish2 {

template<typename Spec>
class CardiCoordGetter {

  std::reference_wrapper<const dish2::ThreadWorld<Spec>> thread_world;

public:

  using value_type = std::tuple<size_t, size_t>;

  template< typename... Args >
  CardiCoordGetter(
    const dish2::ThreadWorld<Spec>& thread_world_,
    Args&&...
  )
  : thread_world(thread_world_)
  {}

  const value_type Get(
    const size_t cell_idx, const size_t cardinal_idx=0
  ) const {
    return std::make_tuple( cell_idx, cardinal_idx );
  }

  size_t GetNumCells() const { return thread_world.get().GetSize(); }

  size_t GetNumCardinals( const size_t cell_idx=0 ) const {
    return thread_world.get().GetCell( cell_idx ).GetNumCardinals();
  }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_GETTERS_CARDICOORDGETTER_HPP_INCLUDE
