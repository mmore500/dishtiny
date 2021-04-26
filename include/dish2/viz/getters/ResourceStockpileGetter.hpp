#pragma once
#ifndef DISH2_VIZ_GETTERS_RESOURCESTOCKPILEGETTER_HPP_INCLUDE
#define DISH2_VIZ_GETTERS_RESOURCESTOCKPILEGETTER_HPP_INCLUDE

#include "../../../../third-party/Empirical/include/emp/datastructs/hash_utils.hpp"
#include "../../../../third-party/Empirical/include/emp/math/math.hpp"
#include "../../../../third-party/Empirical/include/emp/web/color_map.hpp"

#include "../../cell/cardinal_iterators/ResourceStockpileWrapper.hpp"
#include "../../world/ThreadWorld.hpp"

#include "CardinalIteratorAdapter.hpp"

namespace dish2 {

template<typename Spec>
class ResourceStockpileGetter
: public dish2::CardinalIteratorAdapter<Spec, dish2::ResourceStockpileWrapper> {

  using parent_t
    = dish2::CardinalIteratorAdapter<Spec, dish2::ResourceStockpileWrapper>;

public:
  // inherit constructors
  using parent_t::parent_t;

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_GETTERS_RESOURCESTOCKPILEGETTER_HPP_INCLUDE
