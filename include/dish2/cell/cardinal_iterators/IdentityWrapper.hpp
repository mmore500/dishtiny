#pragma once
#ifndef DISH2_CELL_CARDINAL_ITERATORS_IDENTITYWRAPPER_HPP_INCLUDE
#define DISH2_CELL_CARDINAL_ITERATORS_IDENTITYWRAPPER_HPP_INCLUDE

#include "../../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "../Cardinal.hpp"

namespace dish2 {

template<typename Spec>
class IdentityWrapper : public emp::vector<Cardinal<Spec>>::iterator {

  using parent_t = typename emp::vector<Cardinal<Spec>>::iterator;

public:

  IdentityWrapper() = default;

  IdentityWrapper(const parent_t& in) : parent_t(in) {}

};

} // namespace dish2

#endif // #ifndef DISH2_CELL_CARDINAL_ITERATORS_IDENTITYWRAPPER_HPP_INCLUDE
