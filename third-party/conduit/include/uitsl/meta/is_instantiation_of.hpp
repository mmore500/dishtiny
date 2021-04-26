#pragma once
#ifndef UITSL_META_IS_INSTANTIATION_OF_HPP_INCLUDE
#define UITSL_META_IS_INSTANTIATION_OF_HPP_INCLUDE

#include <type_traits>

namespace uitsl {

// adapted from https://stackoverflow.com/a/11251408
template < template <typename...> class Template, typename T >
struct is_instantiation_of : std::false_type {};

template < template <typename...> class Template, typename... Args >
struct is_instantiation_of< Template, Template<Args...> > : std::true_type {};

} // namespace uitsl

#endif // #ifndef UITSL_META_IS_INSTANTIATION_OF_HPP_INCLUDE
