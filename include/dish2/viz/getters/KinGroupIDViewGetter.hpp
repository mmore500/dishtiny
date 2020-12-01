#pragma once
#ifndef DISH2_VIZ_GETTERS_KINGROUPIDVIEWGETTER_HPP_INCLUDE
#define DISH2_VIZ_GETTERS_KINGROUPIDVIEWGETTER_HPP_INCLUDE

#include "../../../../third-party/Empirical/include/emp/datastructs/hash_utils.hpp"
#include "../../../../third-party/Empirical/include/emp/math/math.hpp"
#include "../../../../third-party/Empirical/include/emp/web/color_map.hpp"

#include "../../cell/cardinal_iterators/KinGroupIDViewWrapper.hpp"
#include "../../world/ThreadWorld.hpp"

#include "CardinalIteratorAdapter.hpp"

namespace dish2 {

template<typename Spec>
class KinGroupIDViewGetter
: public dish2::CardinalIteratorAdapter<
  Spec, dish2::KinGroupIDViewWrapper
> {

  using parent_t = dish2::CardinalIteratorAdapter<
    Spec, dish2::KinGroupIDViewWrapper
  >;

public:
  // inherit constructors
  using parent_t::parent_t;

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_GETTERS_KINGROUPIDVIEWGETTER_HPP_INCLUDE
