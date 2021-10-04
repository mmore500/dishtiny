#pragma once
#ifndef DISH2_CONFIG_PRINT_READABLE_STATES_HPP_INCLUDE
#define DISH2_CONFIG_PRINT_READABLE_STATES_HPP_INCLUDE

#include <iostream>

#include "../../../third-party/Empirical/include/emp/base/macros.hpp"

#include "../debug/log_tee.hpp"
#include "../peripheral/readable_state/ReadableState.hpp"

#include "cfg.hpp"

namespace dish2 {

template< typename Spec >
void print_readable_states() {

  dish2::log_tee << "==============================" << '\n';
  dish2::log_tee << "|      Readable States       |" << '\n';
  dish2::log_tee << "==============================" << '\n';

  const size_t num_writables = dish2::ReadableState<Spec>::parent_size;
  for (size_t i{}; i < num_writables; ++i) {
    dish2::log_tee
      << i << ' '
      << dish2::ReadableState< Spec >::GetLeafTypeName( i ) << '\n';
  }

  dish2::log_tee << "==============================\n" << '\n';

}

} // namespace dish2

#endif // #ifndef DISH2_CONFIG_PRINT_READABLE_STATES_HPP_INCLUDE
