#pragma once
#ifndef DISH2_CELL_SERVICE_IMPLS_INTERMESSAGELAUNCHINGSERVICE_HPP_INCLUDE
#define DISH2_CELL_SERVICE_IMPLS_INTERMESSAGELAUNCHINGSERVICE_HPP_INCLUDE

#include "../cardinal_iterators/CpuWrapper.hpp"

namespace dish2 {

template <class Spec>
void Cell<Spec>::InterMessageLaunchingService() {

  static uitsl::WarnOnce warning{
    "TODO InterMessageLaunchingService unimplemented"
  };

}

} // namespace dish2

#endif // #ifndef DISH2_CELL_SERVICE_IMPLS_INTERMESSAGELAUNCHINGSERVICE_HPP_INCLUDE
