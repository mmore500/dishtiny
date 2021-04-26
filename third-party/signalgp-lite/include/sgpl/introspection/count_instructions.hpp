#pragma once
#ifndef SGPL_INTROSPECTION_COUNT_INSTRUCTIONS_HPP_INCLUDE
#define SGPL_INTROSPECTION_COUNT_INSTRUCTIONS_HPP_INCLUDE

#include <algorithm>
#include <string>

#include "../program/GlobalAnchorIterator.hpp"
#include "../program/Program.hpp"

namespace sgpl {

template<typename Spec>
size_t count_instructions(
  const sgpl::Program<Spec>& program, const std::string& category
) {

  return std::count_if(
    std::begin( program ), std::end( program ),
    [&]( const auto& inst ){ return inst.GetCategories().count( category ); }
  );

}

} // namespace sgpl

#endif // #ifndef SGPL_INTROSPECTION_COUNT_INSTRUCTIONS_HPP_INCLUDE
