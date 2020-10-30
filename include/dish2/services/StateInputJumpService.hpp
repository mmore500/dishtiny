#pragma once
#ifndef DISH2_CELL_SERVICES_STATEINPUTJUMPSERVICE_HPP_INCLUDE
#define DISH2_CELL_SERVICES_STATEINPUTJUMPSERVICE_HPP_INCLUDE

#include <set>
#include <utility>

#include "../../../third-party/conduit/include/uitsl/math/shift_mod.hpp"

#include "../cell/cardinal_iterators/StateNodeInputWrapper.hpp"
#include "../config/cfg.hpp"

namespace dish2 {

struct StateInputJumpService {

  static bool ShouldRun( const size_t update, const bool alive ) {
    const size_t freq = dish2::cfg.STATE_INPUT_JUMP_SERVICE_FREQUENCY();
    return
      freq > 0
      && uitsl::shift_mod( update, freq ) == 0;
  }

  template<typename Cell>
  static void DoService( Cell& cell ) {

    std::for_each(
      cell.template begin<dish2::StateNodeInputWrapper<Spec>>(),
      cell.template end<dish2::StateNodeInputWrapper<Spec>>(),
      []( auto& input ){ input.Jump(); }
    );

  }

};

} // namespace dish2

#endif // #ifndef DISH2_CELL_SERVICES_STATEINPUTJUMPSERVICE_HPP_INCLUDE
