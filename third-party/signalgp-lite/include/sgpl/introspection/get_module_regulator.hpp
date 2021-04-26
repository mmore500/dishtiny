#pragma once
#ifndef SGPL_INTROSPECTION_GET_MODULE_REGULATOR_HPP_INCLUDE
#define SGPL_INTROSPECTION_GET_MODULE_REGULATOR_HPP_INCLUDE

#include <algorithm>

#include "../hardware/Cpu.hpp"
#include "../program/Program.hpp"
#include "../utility/CountingIterator.hpp"

#include "get_module_pos.hpp"

namespace sgpl {

template<typename Spec>
emp::optional<float> get_module_regulator(
  const sgpl::Cpu<Spec>& cpu,
  const sgpl::Program<Spec>& program,
  const size_t module_idx,
  const size_t jump_table_idx=0
) {

  using library_t = typename Spec::library_t;

  const auto& jump_table = cpu.GetGlobalJumpTable( jump_table_idx );

  // cpu's jump table has not yet been initialized
  if ( jump_table.GetSize() == 0 ) return 0.0f;

  const size_t module_pos = sgpl::get_module_pos<Spec>( program, module_idx );

  // edge cases: the last and first program positions might not
  // correspond to valid modules
  if ( module_pos == program.size() ) return 0.0f;
  else if (
    module_pos == 0
    && !library_t::IsAnchorGlobalOpCode( program.front().op_code )
  ) return 0.0f;

  if ( !jump_table.HasVal( module_pos ) ) return std::nullopt;

  const size_t module_uid = jump_table.GetUid( module_pos );

  return jump_table.ViewRegulator( module_uid );

}

} // namespace sgpl

#endif // #ifndef SGPL_INTROSPECTION_GET_MODULE_REGULATOR_HPP_INCLUDE
