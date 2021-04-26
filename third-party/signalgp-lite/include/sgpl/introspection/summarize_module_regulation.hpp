#pragma once
#ifndef SGPL_INTROSPECTION_SUMMARIZE_MODULE_REGULATION_HPP_INCLUDE
#define SGPL_INTROSPECTION_SUMMARIZE_MODULE_REGULATION_HPP_INCLUDE

#include <algorithm>

#include "../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "../hardware/Cpu.hpp"
#include "../program/Program.hpp"
#include "../utility/CountingIterator.hpp"

#include "count_modules.hpp"
#include "get_module_regulator.hpp"

namespace sgpl {

// index represents module ID, value represents regulation
template<typename Spec>
emp::vector<float> summarize_module_regulation(
  const sgpl::Cpu<Spec>& cpu,
  const sgpl::Program<Spec>& program,
  const size_t jump_table_idx=0
) {

  emp::vector<float> res;

  std::transform(
    sgpl::CountingIterator{},
    sgpl::CountingIterator{ sgpl::count_modules<Spec>( program ) },
    std::back_inserter( res ),
    [&](const auto& module_idx){
      return sgpl::get_module_regulator<Spec>(
        cpu,
        program,
        module_idx,
        jump_table_idx
      ).value_or( 0.0f );
    }
  );

  return res;

}

} // namespace sgpl

#endif // #ifndef SGPL_INTROSPECTION_SUMMARIZE_MODULE_REGULATION_HPP_INCLUDE
