#pragma once
#ifndef DISH2_SPEC_SPEC_HPP_INCLUDE
#define DISH2_SPEC_SPEC_HPP_INCLUDE

#include "../../../third-party/signalgp-lite/include/sgpl/config/Spec.hpp"
#include "../peripheral/Peripheral.hpp"

namespace dish2 {

struct Spec {

  using sgpl_spec_t = sgpl::Spec<
    sgpl::CompleteOpLibrary,
    dish2::Peripheral
  >;

};


} // namespace dish2

#endif // #ifndef DISH2_SPEC_SPEC_HPP_INCLUDE
