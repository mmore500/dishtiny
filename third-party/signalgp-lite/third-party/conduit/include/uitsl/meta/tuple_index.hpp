#pragma once
#ifndef UITSL_META_TUPLE_INDEX_HPP_INCLUDE
#define UITSL_META_TUPLE_INDEX_HPP_INCLUDE

#include <type_traits>

namespace uitsl {

// adapted from https://stackoverflow.com/a/18063608

template <class T, class Tuple>
struct tuple_index;

template <class T, class... Types>
struct tuple_index<T, std::tuple<T, Types...>> {
    static constexpr std::size_t value = 0;
};

template <class T, class U, class... Types>
struct tuple_index<T, std::tuple<U, Types...>> {
    static constexpr std::size_t value =
      1 + tuple_index<T, std::tuple<Types...>>::value;
};
} // namespace uitsl

#endif // #ifndef UITSL_META_TUPLE_INDEX_HPP_INCLUDE
