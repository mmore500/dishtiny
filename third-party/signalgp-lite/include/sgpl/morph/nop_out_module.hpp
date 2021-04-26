#pragma once
#ifndef SGPL_MORPH_NOP_OUT_MODULE_HPP_INCLUDE
#define SGPL_MORPH_NOP_OUT_MODULE_HPP_INCLUDE

#include <iterator>

#include "../../../third-party/Empirical/include/emp/base/assert.hpp"
#include "../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "../introspection/count_modules.hpp"
#include "../introspection/get_module_length.hpp"
#include "../introspection/get_module_pos.hpp"
#include "../program/GlobalAnchorIterator.hpp"
#include "../program/Program.hpp"

#include "nop_out_instructions.hpp"

namespace sgpl {

template< typename Spec >
sgpl::Program<Spec> nop_out_module(
  sgpl::Program<Spec> program, const size_t module_idx
) {

  emp_assert( module_idx < sgpl::count_modules<Spec>( program ) );

  const size_t module_pos = sgpl::get_module_pos( program, module_idx );
  const size_t module_length = sgpl::get_module_length( program, module_idx );

  emp::vector< char > should_nop( module_pos, false );

  const emp::vector< char > module( module_length, true );
  should_nop.insert(std::end(should_nop), std::begin(module), std::end(module));

  should_nop.resize( program.size(), false );

  return sgpl::nop_out_instructions<Spec>( program, should_nop );

}

} // namespace sgpl

#endif // #ifndef SGPL_MORPH_NOP_OUT_MODULE_HPP_INCLUDE
