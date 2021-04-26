#pragma once
#ifndef UITSL_META_COMMON_SIGNED_T_HPP_INCLUDE
#define UITSL_META_COMMON_SIGNED_T_HPP_INCLUDE

// adapted from https://stackoverflow.com/a/63436491

#include <type_traits>

namespace uitsl {

/**
 * Similar to std::common_type_t<A, B>, but if A or B are signed, the result will also be signed.
 *
 * This differs from the regular type promotion rules, where signed types are promoted to unsigned types.
 */
template <typename A, typename B>
using common_signed_t =
    std::conditional_t<std::is_unsigned_v<A> && std::is_unsigned_v<B>,
                       std::common_type_t<A, B>,
                       std::common_type_t<std::make_signed_t<A>, std::make_signed_t<B>>>;

} // namespace uitsl

#endif // #ifndef UITSL_META_COMMON_SIGNED_T_HPP_INCLUDE
