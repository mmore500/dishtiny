#pragma once
#ifndef SGPL_INTROSPECTION_COUNT_NOP_INSTRUCTIONS_HPP_INCLUDE
#define SGPL_INTROSPECTION_COUNT_NOP_INSTRUCTIONS_HPP_INCLUDE

#include "../../../third-party/Empirical/include/emp/base/assert.hpp"

#include "../program/Program.hpp"

#include "count_instructions.hpp"

namespace sgpl {

template<typename Spec>
size_t count_nop_instructions( const sgpl::Program<Spec>& program ) {

  emp_assert(
    sgpl::count_instructions<Spec>( program, "op" )
    == program.size() - sgpl::count_instructions<Spec>( program, "nop" )
  );

  return sgpl::count_instructions<Spec>( program, "nop" );

}

} // namespace sgpl

#endif // #ifndef SGPL_INTROSPECTION_COUNT_NOP_INSTRUCTIONS_HPP_INCLUDE
