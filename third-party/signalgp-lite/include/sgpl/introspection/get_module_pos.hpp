#pragma once
#ifndef SGPL_INTROSPECTION_GET_MODULE_POS_HPP_INCLUDE
#define SGPL_INTROSPECTION_GET_MODULE_POS_HPP_INCLUDE

#include "../program/GlobalAnchorIterator.hpp"
#include "../program/Program.hpp"

#include "count_modules.hpp"

namespace sgpl {

template<typename Spec>
size_t get_module_pos(
  const sgpl::Program<Spec>& program,
  const size_t module_idx
) {

  emp_assert( module_idx < sgpl::count_modules( program ) );

  const size_t raw_distance = std::next(
    sgpl::GlobalAnchorIterator<Spec>::make_begin( program ),
    module_idx
  ).CalcDistance( std::begin( program ) );

  return raw_distance;

}

} // namespace sgpl

#endif // #ifndef SGPL_INTROSPECTION_GET_MODULE_POS_HPP_INCLUDE
