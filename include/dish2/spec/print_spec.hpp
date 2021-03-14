#pragma once
#ifndef DISH2_SPEC_PRINT_SPEC_HPP_INCLUDE
#define DISH2_SPEC_PRINT_SPEC_HPP_INCLUDE

#include <iostream>

#include "../../../third-party/Empirical/include/emp/base/macros.hpp"

#include "../peripheral/readable_state/ReadableState.hpp"
#include "../peripheral/readable_state/writable_state/WritableState.hpp"

namespace dish2 {

template<typename Spec>
void print_spec() {

  std::cout << "==============================" << std::endl;
  std::cout << "|     How am I compiled?     |" << std::endl;
  std::cout << "==============================" << std::endl;
  std::cout << "SOURCE DIRECTORY "
    << EMP_STRINGIFY(DISHTINY_SOURCE_DIR_) << std::endl;
  std::cout << "SOURCE HASH " << EMP_STRINGIFY(DISHTINY_HASH_) << std::endl;
  std::cout << "------------------------------" << std::endl;

  constexpr size_t readable_size = dish2::ReadableState<Spec>::parent_size;
  constexpr size_t writable_size = dish2::WritableState<Spec>::parent_size;
  std::cout << "ReadableState size: " << readable_size << std::endl;
  std::cout << "WritableState size: " << writable_size << std::endl;

  std::cout << "==============================\n" << std::endl;

}

} // namespace dish2

#endif // #ifndef DISH2_SPEC_PRINT_SPEC_HPP_INCLUDE
