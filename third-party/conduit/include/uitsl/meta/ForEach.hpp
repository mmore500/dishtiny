#pragma once
#ifndef UITSL_META_FOREACH_HPP_INCLUDE
#define UITSL_META_FOREACH_HPP_INCLUDE

#include <functional>
#include <stddef.h>

namespace uitsl {

template <
  template<size_t> typename Payload,
  size_t N,
  size_t Operand=1,
  typename Afterthought=std::plus<size_t>,
  typename Comparison=std::less<size_t>
  >
struct ForEach {

  template <size_t I>
  void item() {

    if constexpr ( Comparison{}(I, N) ) {
      Payload<I>{};

      constexpr size_t next = Afterthought{}(
        I,
        Operand
      );

      // recurse upwards
      ForEach<
        Payload,
        N,
        Operand,
        Afterthought,
        Comparison
      >{}.template item<next>();
    }

  }

};

} // namespace uitsl

#endif // #ifndef UITSL_META_FOREACH_HPP_INCLUDE
