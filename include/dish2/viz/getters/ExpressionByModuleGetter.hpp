#pragma once
#ifndef DISH2_VIZ_GETTERS_EXPRESSIONBYMODULEGETTER_HPP_INCLUDE
#define DISH2_VIZ_GETTERS_EXPRESSIONBYMODULEGETTER_HPP_INCLUDE

#include <functional>

#include "../../../../third-party/signalgp-lite/include/sgpl/introspection/count_cores_with_module_idx.hpp"

#include "../../cell/cardinal_iterators/CpuWrapper.hpp"
#include "../../cell/Cell.hpp"
#include "../../config/cfg.hpp"
#include "../../genome/Genome.hpp"
#include "../../genome/KinGroupID.hpp"
#include "../../world/ThreadWorld.hpp"

#include "GenomeGetter.hpp"

namespace dish2 {

template<typename Spec>
class ExpressionByModuleGetter {

  dish2::CardinalIteratorAdapter<Spec, dish2::CpuWrapper> cpu_getter;
  dish2::GenomeGetter<Spec> genome_getter;

  size_t module_idx;

public:

  using value_type = size_t;

  template< typename... Args >
  ExpressionByModuleGetter(
    const dish2::ThreadWorld<Spec>& thread_world_,
    const size_t idx,
    Args&&...
  )
  : cpu_getter(thread_world_)
  , genome_getter(thread_world_)
  , module_idx( idx )
  {}

  const value_type Get(
    const size_t cell_idx, const size_t cardinal_idx=0
  ) const {

    const auto& cpu = cpu_getter.Get( cell_idx, cardinal_idx );
    const auto& program = genome_getter.Get( cell_idx, cardinal_idx ).program;

    return sgpl::count_cores_with_module_idx(
      cpu,
      program,
      module_idx
    );

  }

  size_t GetNumCells() const { return cpu_getter.GetNumCells(); }

  size_t GetNumCardinals( const size_t cell_idx=0 ) const {
    return cpu_getter.GetNumCardinals();
  }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_GETTERS_EXPRESSIONBYMODULEGETTER_HPP_INCLUDE
