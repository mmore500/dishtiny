#pragma once
#ifndef UIT_DUCTS_PROC_IMPL_INLET_ACCUMULATING_TYPE_SPAN_S__ISENDDUCT_HPP_INCLUDE
#define UIT_DUCTS_PROC_IMPL_INLET_ACCUMULATING_TYPE_SPAN_S__ISENDDUCT_HPP_INCLUDE

#include "impl/AccumulatingSpanIsendDuct.hpp"

namespace uit {
namespace s {

template<typename ImplSpec>
class IsendDuct
: public internal::AccumulatingSpanIsendDuct<ImplSpec, false>
{

  using parent_t = internal::AccumulatingSpanIsendDuct<ImplSpec, false>;

public:

  // inherit parent's constructors
  using parent_t::parent_t;

};


} // namespace t
} // namespace uit

#endif // #ifndef UIT_DUCTS_PROC_IMPL_INLET_ACCUMULATING_TYPE_SPAN_S__ISENDDUCT_HPP_INCLUDE
