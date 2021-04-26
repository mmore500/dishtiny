#pragma once
#ifndef SGPL_UTILITY_COUNT_OPERATION_RANDOM_TOUCHES_HPP_INCLUDE
#define SGPL_UTILITY_COUNT_OPERATION_RANDOM_TOUCHES_HPP_INCLUDE

#include <algorithm>
#include <cstddef>
#include <functional>
#include <type_traits>

#include "../../../third-party/Empirical/include/emp/base/optional.hpp"
#include "../../../third-party/Empirical/include/emp/math/Random.hpp"
#include "../../../third-party/Empirical/include/emp/math/random_utils.hpp"

#include "../hardware/Cpu.hpp"
#include "../program/Instruction.hpp"
#include "../program/Program.hpp"

#include "count_thread_local_random_touches.hpp"

namespace sgpl {

template< typename Operation, typename Spec >
size_t count_operation_random_touches() {

  const auto routine = [](){
    using cpu_t = sgpl::Cpu<Spec>;
    cpu_t cpu;
    cpu.DoLaunchCore();

    using peripheral_t = typename Spec::peripheral_t;

    emp::optional< peripheral_t > peripheral;
    if constexpr ( std::is_default_constructible< peripheral_t >::value ) {
      peripheral.emplace();
    } else {
      peripheral = peripheral_t::make_dummy();
    }

    Operation::template run<Spec>( cpu.GetActiveCore(), {}, {}, *peripheral );
  };

  thread_local const size_t res
    = sgpl::count_thread_local_random_touches( routine );

  emp_assert( res == sgpl::count_thread_local_random_touches( routine ) );

  return res;

}

} // namespace sgpl

#endif // #ifndef SGPL_UTILITY_COUNT_OPERATION_RANDOM_TOUCHES_HPP_INCLUDE
