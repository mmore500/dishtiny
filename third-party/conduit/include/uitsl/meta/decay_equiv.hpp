#pragma once
#ifndef UITSL_META_DECAY_EQUIV_HPP_INCLUDE
#define UITSL_META_DECAY_EQUIV_HPP_INCLUDE

#include <type_traits>

namespace uitsl {

// adapted from https://en.cppreference.com/w/cpp/types/decay
template <typename T, typename U>
struct decay_equiv : std::is_same<typename std::decay<T>::type, U>::type
{};

} // namespace uitsl

#endif // #ifndef UITSL_META_DECAY_EQUIV_HPP_INCLUDE
