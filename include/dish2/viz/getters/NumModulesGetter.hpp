#pragma once
#ifndef DISH2_VIZ_GETTERS_NUMMODULESGETTER_HPP_INCLUDE
#define DISH2_VIZ_GETTERS_NUMMODULESGETTER_HPP_INCLUDE

#include <functional>

#include "../../../../third-party/signalgp-lite/include/sgpl/introspection/count_modules.hpp"

#include "GenomeGetter.hpp"

namespace dish2 {

template<typename Spec>
class NumModulesGetter {

  GenomeGetter<Spec> getter;

  using sgpl_spec_t = typename Spec::sgpl_spec_t;

public:

  using value_type = size_t;

  template< typename... Args >
  NumModulesGetter(Args&&... args)
  : getter( std::forward<Args>(args)... )
  {}

  const value_type Get(
    const size_t cell_idx, const size_t cardinal_idx=0
  ) const {
    const auto& program = getter.Get(
      cell_idx, cardinal_idx
    ).program;

    return sgpl::count_modules<sgpl_spec_t>( program );
  }

  size_t GetNumCells() const { return getter.GetNumCells(); }

  size_t GetNumCardinals( const size_t cell_idx=0 ) const {
    return getter.GetNumCardinals( cell_idx );
  }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_GETTERS_NUMMODULESGETTER_HPP_INCLUDE
