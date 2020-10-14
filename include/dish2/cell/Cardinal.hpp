#pragma once
#ifndef DISH__CELL_CARDINAL_HPP_INCLUDE
#define DISH__CELL_CARDINAL_HPP_INCLUDE

#include "../../../third-party/signalgp-lite/include/sgpl/hardware/Cpu.hpp"

#include "../peripheral/Peripheral.hpp"

namespace dish2 {

template<typename Spec>
struct Cardinal {

  using sgpl_spec_t = typename Spec::sgpl_spec_t;

  sgpl::Cpu<sgpl_spec_t> cpu;

  dish2::Peripheral peripheral;

};

} // namespace dish2

#endif // #ifndef DISH__CELL_CARDINAL_HPP_INCLUDE
