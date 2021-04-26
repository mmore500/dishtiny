#pragma once
#ifndef SGPL_INTROSPECTION_GET_CUR_MODULE_IDX_HPP_INCLUDE
#define SGPL_INTROSPECTION_GET_CUR_MODULE_IDX_HPP_INCLUDE

#include <iterator>

#include "../hardware/Core.hpp"
#include "../program/GlobalAnchorIterator.hpp"
#include "../program/Program.hpp"

namespace sgpl {

template<typename Spec>
size_t get_cur_module_idx(
  const sgpl::Core<Spec>& core,
  const sgpl::Program<Spec>& program
) {

  if ( core.HasTerminated() ) return std::numeric_limits<size_t>::max();

  size_t res{};

  for (
    auto it = sgpl::GlobalAnchorIterator<Spec>::make_begin( program );
    it.CalcDistance( std::begin( program ) ) < core.GetProgramCounter();
    ++it
  ) ++res;

  return res;

}

} // namespace sgpl

#endif // #ifndef SGPL_INTROSPECTION_GET_CUR_MODULE_IDX_HPP_INCLUDE
