#pragma once
#ifndef DISH2_CELL_SERVICE_IMPLS_EVENTLAUNCHINGSERVICE_HPP_INCLUDE
#define DISH2_CELL_SERVICE_IMPLS_EVENTLAUNCHINGSERVICE_HPP_INCLUDE

#include <algorithm>

#include "../../../../third-party/conduit/include/uitsl/debug/WarnOnce.hpp"

#include "../cardinal_iterators/CpuWrapper.hpp"

namespace dish2 {

template <class Spec>
void Cell<Spec>::EventLaunchingService() {

  std::for_each(
    begin<dish2::CpuWrapper<Spec>>(),
    end<dish2::CpuWrapper<Spec>>(),
    [](auto& cpu){ cpu.TryLaunchCore(); }
  );

  static uitsl::WarnOnce warning{ "TODO EventLaunchingService unimplemented" };

}

} // namespace dish2

#endif // #ifndef DISH2_CELL_SERVICE_IMPLS_EVENTLAUNCHINGSERVICE_HPP_INCLUDE
