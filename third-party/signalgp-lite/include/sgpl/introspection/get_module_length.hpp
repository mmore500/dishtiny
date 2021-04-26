#pragma once
#ifndef SGPL_INTROSPECTION_GET_MODULE_LENGTH_HPP_INCLUDE
#define SGPL_INTROSPECTION_GET_MODULE_LENGTH_HPP_INCLUDE

#include "../program/GlobalAnchorIterator.hpp"
#include "../program/Program.hpp"

#include "get_module_pos.hpp"

namespace sgpl {

template<typename Spec>
size_t get_module_length(
  const sgpl::Program<Spec>& program,
  const size_t module_idx
) {

  emp_assert( sgpl::count_modules( program ) );

  if ( module_idx + 1 == sgpl::count_modules( program ) ) {
    // if last module, get distance to last inst instead of to next module
    return program.size() - sgpl::get_module_pos<Spec>( program, module_idx );
  } else return (
    sgpl::get_module_pos<Spec>( program, module_idx + 1 )
    - sgpl::get_module_pos<Spec>( program, module_idx )
  );

}

} // namespace sgpl

#endif // #ifndef SGPL_INTROSPECTION_GET_MODULE_LENGTH_HPP_INCLUDE
