#pragma once
#ifndef UIT_DUCTS_PROC_IMPL_INLET_ACCUMULATING_TYPE_TRIVIAL_T__ISENDDUCT_HPP_INCLUDE
#define UIT_DUCTS_PROC_IMPL_INLET_ACCUMULATING_TYPE_TRIVIAL_T__ISENDDUCT_HPP_INCLUDE

#include "impl/AccumulatingTrivialIsendDuct.hpp"

namespace uit {
namespace t {

template<typename ImplSpec>
class IsendDuct
: public internal::AccumulatingTrivialIsendDuct<ImplSpec, false>
{

  using parent_t = internal::AccumulatingTrivialIsendDuct<ImplSpec, false>;

public:

  // inherit parent's constructors
  using parent_t::parent_t;

};


} // namespace t
} // namespace uit

#endif // #ifndef UIT_DUCTS_PROC_IMPL_INLET_ACCUMULATING_TYPE_TRIVIAL_T__ISENDDUCT_HPP_INCLUDE
