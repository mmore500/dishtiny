#pragma once
#ifndef DISH2_SPEC_PRINT_SPEC_HPP_INCLUDE
#define DISH2_SPEC_PRINT_SPEC_HPP_INCLUDE

#include <iostream>

#include "../../../third-party/Empirical/include/emp/base/macros.hpp"

#include "../debug/log_tee.hpp"
#include "../peripheral/readable_state/introspective_state/IntrospectiveState.hpp"
#include "../peripheral/readable_state/ReadableState.hpp"
#include "../peripheral/readable_state/writable_state/WritableState.hpp"
#include "../spec/Spec.hpp"

namespace dish2 {

template<typename Spec>
void print_spec() {

  dish2::log_tee << "==============================" << '\n';
  dish2::log_tee << "|     How am I compiled?     |" << '\n';
  dish2::log_tee << "==============================" << '\n';
  dish2::log_tee << "SOURCE DIRECTORY "
    << EMP_STRINGIFY(DISHTINY_SOURCE_DIR_) << '\n';
  dish2::log_tee << "SOURCE HASH " << EMP_STRINGIFY(DISHTINY_HASH_) << '\n';
  dish2::log_tee << "DISH2_SPEC " << EMP_STRINGIFY(DISH2_SPEC) << '\n';
  dish2::log_tee << "------------------------------" << '\n';

  constexpr size_t introspective_size
    = dish2::IntrospectiveState<Spec>::parent_size;
  constexpr size_t readable_size = dish2::ReadableState<Spec>::parent_size;
  constexpr size_t writable_size = dish2::WritableState<Spec>::parent_size;
  dish2::log_tee << "IntrospectiveState size: " << introspective_size << '\n';
  dish2::log_tee << "ReadableState size: " << readable_size << '\n';
  dish2::log_tee << "WritableState size: " << writable_size << '\n';

  constexpr size_t introspective_bytes
    = sizeof( dish2::IntrospectiveState<Spec> );
  constexpr size_t readable_bytes = sizeof( dish2::ReadableState<Spec> );
  constexpr size_t writable_bytes = sizeof( dish2::WritableState<Spec> );
  dish2::log_tee << "IntrospectiveState bytes: " << introspective_bytes << '\n';
  dish2::log_tee << "ReadableState bytes: " << readable_bytes << '\n';
  dish2::log_tee << "WritableState bytes: " << writable_bytes << '\n';

  dish2::log_tee << "==============================\n" << '\n';

}

} // namespace dish2

#endif // #ifndef DISH2_SPEC_PRINT_SPEC_HPP_INCLUDE
