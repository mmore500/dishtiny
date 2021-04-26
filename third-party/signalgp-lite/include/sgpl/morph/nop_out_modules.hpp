#pragma once
#ifndef SGPL_MORPH_NOP_OUT_MODULES_HPP_INCLUDE
#define SGPL_MORPH_NOP_OUT_MODULES_HPP_INCLUDE

#include <algorithm>

#include "../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "../introspection/count_modules.hpp"
#include "../introspection/make_module_mask.hpp"
#include "../program/Program.hpp"

#include "nop_out_instructions.hpp"

namespace sgpl {

template< typename Spec >
sgpl::Program<Spec> nop_out_modules(
  sgpl::Program<Spec> program, const emp::vector<char> modulewise_should_nop
) {

  emp_assert(
    sgpl::count_modules<Spec>( program ) == modulewise_should_nop.size()
  );

  emp::vector<char> instructionwise_should_nop( program.size(), false );

  const size_t num_modules = modulewise_should_nop.size();
  for (size_t module_idx{}; module_idx < num_modules; ++module_idx) {

    if ( modulewise_should_nop[ module_idx ] ) {
      const auto is_module_mask = sgpl::make_module_mask( program, module_idx );
      std::transform(
        std::begin( instructionwise_should_nop ),
        std::end( instructionwise_should_nop ),
        std::begin( is_module_mask ),
        std::begin( instructionwise_should_nop ),
        []( const bool should_nop, const bool is_module ){
          return should_nop || is_module;
        }
      );
    }

  }

  return sgpl::nop_out_instructions<Spec>(program, instructionwise_should_nop);

}

} // namespace sgpl

#endif // #ifndef SGPL_MORPH_NOP_OUT_MODULES_HPP_INCLUDE
