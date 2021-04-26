#pragma once
#ifndef SGPL_MORPH_NOP_OUT_INSTRUCTION_CATEGORY_HPP_INCLUDE
#define SGPL_MORPH_NOP_OUT_INSTRUCTION_CATEGORY_HPP_INCLUDE

#include <string>

#include "../program/Program.hpp"

namespace sgpl {

template<typename Spec>
sgpl::Program<Spec> nop_out_instruction_category(
  sgpl::Program<Spec> program, const std::string& category
) {

  for ( auto& inst : program ) {
    if ( inst.GetCategories().count( category ) ) inst.NopOut();
  }

  return program;

}

} // namespace sgpl

#endif // #ifndef SGPL_MORPH_NOP_OUT_INSTRUCTION_CATEGORY_HPP_INCLUDE
