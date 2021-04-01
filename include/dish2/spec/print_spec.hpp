#pragma once
#ifndef DISH2_SPEC_PRINT_SPEC_HPP_INCLUDE
#define DISH2_SPEC_PRINT_SPEC_HPP_INCLUDE

#include <iostream>

#include "../../../third-party/Empirical/include/emp/base/macros.hpp"

#include "../peripheral/readable_state/introspective_state/IntrospectiveState.hpp"
#include "../peripheral/readable_state/ReadableState.hpp"
#include "../peripheral/readable_state/writable_state/WritableState.hpp"
#include "../spec/Spec.hpp"

namespace dish2 {

template<typename Spec>
void print_spec() {

  std::cout << "==============================" << '\n';
  std::cout << "|     How am I compiled?     |" << '\n';
  std::cout << "==============================" << '\n';
  std::cout << "SOURCE DIRECTORY "
    << EMP_STRINGIFY(DISHTINY_SOURCE_DIR_) << '\n';
  std::cout << "SOURCE HASH " << EMP_STRINGIFY(DISHTINY_HASH_) << '\n';
  std::cout << "DISH2_SPEC " << EMP_STRINGIFY(DISH2_SPEC) << '\n';
  std::cout << "------------------------------" << '\n';

  constexpr size_t introspective_size
    = dish2::IntrospectiveState<Spec>::parent_size;
  constexpr size_t readable_size = dish2::ReadableState<Spec>::parent_size;
  constexpr size_t writable_size = dish2::WritableState<Spec>::parent_size;
  std::cout << "IntrospectiveState size: " << introspective_size << '\n';
  std::cout << "ReadableState size: " << readable_size << '\n';
  std::cout << "WritableState size: " << writable_size << '\n';

  std::cout << "==============================\n" << '\n';

}

} // namespace dish2

#endif // #ifndef DISH2_SPEC_PRINT_SPEC_HPP_INCLUDE
