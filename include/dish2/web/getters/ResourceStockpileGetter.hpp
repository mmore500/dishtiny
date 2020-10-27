#pragma once
#ifndef DISH2_WEB_GETTERS_RESOURCESTOCKPILEGETTER_HPP_INCLUDE
#define DISH2_WEB_GETTERS_RESOURCESTOCKPILEGETTER_HPP_INCLUDE

#include "../../../../third-party/Empirical/source/tools/hash_utils.h"
#include "../../../../third-party/Empirical/source/tools/math.h"
#include "../../../../third-party/Empirical/source/web/color_map.h"

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

#endif // #ifndef DISH2_WEB_GETTERS_RESOURCESTOCKPILEGETTER_HPP_INCLUDE
