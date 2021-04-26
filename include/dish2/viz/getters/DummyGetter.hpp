#pragma once
#ifndef DISH2_VIZ_GETTERS_DUMMYGETTER_HPP_INCLUDE
#define DISH2_VIZ_GETTERS_DUMMYGETTER_HPP_INCLUDE

#include "../../config/cfg.hpp"
#include "../../config/num_cells_local.hpp"

namespace dish2 {

template<typename T>
class DummyGetter {

  T val;

public:

  using value_type = T;

  DummyGetter(const T& val_=T{}) : val(val_) {}

  const value_type& Get(
    const size_t cell_idx, const size_t cardinal_idx=0
  ) const { return val; }

  size_t GetNumCells() const { return dish2::num_cells_local(); }

  size_t GetNumCardinals( const size_t cell_idx=0 ) const { return 1; }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_GETTERS_DUMMYGETTER_HPP_INCLUDE
