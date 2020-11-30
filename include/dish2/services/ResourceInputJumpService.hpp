#pragma once
#ifndef DISH2_SERVICES_RESOURCEINPUTJUMPSERVICE_HPP_INCLUDE
#define DISH2_SERVICES_RESOURCEINPUTJUMPSERVICE_HPP_INCLUDE

#include <cmath>
#include <functional>
#include <limits>
#include <set>
#include <utility>

#include "../../../third-party/Empirical/source/base/vector.h"
#include "../../../third-party/conduit/include/uitsl/debug/err_audit.hpp"
#include "../../../third-party/conduit/include/uitsl/algorithm/nan_to_zero.hpp"
#include "../../../third-party/conduit/include/uitsl/math/shift_mod.hpp"
#include "../../../third-party/conduit/include/uitsl/polyfill/isfinite.hpp"

#include "../cell/cardinal_iterators/NeighborResourceReceiveResistanceWrapper.hpp"
#include "../cell/cardinal_iterators/ResourceNodeInputWrapper.hpp"
#include "../config/cfg.hpp"
#include "../debug/LogScope.hpp"

namespace dish2 {

struct ResourceInputJumpService {

  static bool ShouldRun( const size_t update, const bool alive ) {
    const size_t freq = dish2::cfg.RESOURCE_INPUT_JUMP_SERVICE_FREQUENCY();
    return
      alive
      && freq > 0
      && uitsl::shift_mod( update, freq ) == 0;
  }

  template<typename Cell>
  static void DoService( Cell& cell ) {

    const dish2::LogScope guard{ "resource input jump service" };

    using spec_t = typename Cell::spec_t;

    std::for_each(
      cell.template begin<dish2::ResourceNodeInputWrapper<spec_t>>(),
      cell.template end<dish2::ResourceNodeInputWrapper<spec_t>>(),
      []( auto& input ){ input.Jump(); }
    );

  }

};

} // namespace dish2

#endif // #ifndef DISH2_SERVICES_RESOURCEINPUTJUMPSERVICE_HPP_INCLUDE
