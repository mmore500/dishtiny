#pragma once
#ifndef DISH2_CELL_SERVICE_IMPLS_STATEINPUTJUMPSERVICE_HPP_INCLUDE
#define DISH2_CELL_SERVICE_IMPLS_STATEINPUTJUMPSERVICE_HPP_INCLUDE

#include <set>
#include <utility>

#include "../cardinal_iterators/StateNodeInputWrapper.hpp"

namespace dish2 {

template <class Spec>
void Cell<Spec>::StateInputJumpService() {

  std::for_each(
    begin<dish2::StateNodeInputWrapper<Spec>>(),
    end<dish2::StateNodeInputWrapper<Spec>>(),
    []( auto& input ){ input.Jump(); }
  );

}

} // namespace dish2

#endif // #ifndef DISH2_CELL_SERVICE_IMPLS_STATEINPUTJUMPSERVICE_HPP_INCLUDE
