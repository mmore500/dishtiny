#pragma once
#ifndef SGPL_INTROSPECTION_SUMMARIZE_MODULE_EXPRESSION_HPP_INCLUDE
#define SGPL_INTROSPECTION_SUMMARIZE_MODULE_EXPRESSION_HPP_INCLUDE

#include <algorithm>

#include "../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "../hardware/Cpu.hpp"
#include "../program/Program.hpp"
#include "../utility/CountingIterator.hpp"

#include "count_cores_with_module_idx.hpp"
#include "count_modules.hpp"

namespace sgpl {

// index represents module ID, value represents expression count
template<typename Spec>
emp::vector<size_t> summarize_module_expression(
  const sgpl::Cpu<Spec>& cpu,
  const sgpl::Program<Spec>& program
) {

  emp::vector<size_t> res;

  std::transform(
    sgpl::CountingIterator{},
    sgpl::CountingIterator{ sgpl::count_modules<Spec>( program ) },
    std::back_inserter( res ),
    [&](const auto& module_idx){
      return sgpl::count_cores_with_module_idx<Spec>(
        cpu,
        program,
        module_idx
      );
    }
  );

  return res;

}

} // namespace sgpl

#endif // #ifndef SGPL_INTROSPECTION_SUMMARIZE_MODULE_EXPRESSION_HPP_INCLUDE
