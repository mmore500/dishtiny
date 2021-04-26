#pragma once
#ifndef SGPL_INTROSPECTION_GET_MODULE_TAG_HPP_INCLUDE
#define SGPL_INTROSPECTION_GET_MODULE_TAG_HPP_INCLUDE

#include "../program/GlobalAnchorIterator.hpp"
#include "../program/Program.hpp"

namespace sgpl {

template<typename Spec>
typename Spec::tag_t get_module_tag(
  const sgpl::Program<Spec>& program,
  const size_t module_idx
) {

  return std::next(
    sgpl::GlobalAnchorIterator<Spec>::make_begin( program ),
    module_idx
  )->tag;

}

} // namespace sgpl

#endif // #ifndef SGPL_INTROSPECTION_GET_MODULE_TAG_HPP_INCLUDE
