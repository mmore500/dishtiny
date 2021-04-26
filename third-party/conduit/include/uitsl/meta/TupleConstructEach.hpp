#pragma once
#ifndef UITSL_META_TUPLECONSTRUCTEACH_HPP_INCLUDE
#define UITSL_META_TUPLECONSTRUCTEACH_HPP_INCLUDE

#include <tuple>
#include <utility>

namespace uitsl {

// adapted from https://stackoverflow.com/a/15418923
template<typename> struct TupleConstructEach;

template<typename ...T>
struct TupleConstructEach<std::tuple<T...>> {

  template<typename... Args>
  static std::tuple<T...> Make(Args&&... args) {
    return std::tuple<T...>{ T(
      std::forward<Args>(args)...
    )... };
  }

};

} // namespace uitsl

#endif // #ifndef UITSL_META_TUPLECONSTRUCTEACH_HPP_INCLUDE
