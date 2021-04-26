#pragma once
#ifndef SGPL_INTROSPECTION_COUNT_MODULES_HPP_INCLUDE
#define SGPL_INTROSPECTION_COUNT_MODULES_HPP_INCLUDE

#include <iterator>

#include "../program/GlobalAnchorIterator.hpp"
#include "../program/Program.hpp"

namespace sgpl {

template<typename Spec>
size_t count_modules( const sgpl::Program<Spec>& program ) {

  return std::distance(
    sgpl::GlobalAnchorIterator<Spec>::make_begin( program ),
    sgpl::GlobalAnchorIterator<Spec>::make_end( program )
  );

}

} // namespace sgpl

#endif // #ifndef SGPL_INTROSPECTION_COUNT_MODULES_HPP_INCLUDE
