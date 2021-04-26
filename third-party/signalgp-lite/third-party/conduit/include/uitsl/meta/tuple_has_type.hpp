#pragma once
#ifndef UITSL_META_TUPLE_HAS_TYPE_HPP_INCLUDE
#define UITSL_META_TUPLE_HAS_TYPE_HPP_INCLUDE

#include <type_traits>

namespace uitsl {

template <typename T, typename Tuple>
struct tuple_has_type;

template <typename T, typename... Us>
struct tuple_has_type<T, std::tuple<Us...>>
: std::disjunction<std::is_same<T, Us>...> {};

} // namespace uitsl

#endif // #ifndef UITSL_META_TUPLE_HAS_TYPE_HPP_INCLUDE
