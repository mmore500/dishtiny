#pragma once
#ifndef UITSL_MATH_DIVIDE_UTILS_HPP_INCLUDE
#define UITSL_MATH_DIVIDE_UTILS_HPP_INCLUDE

#include <type_traits>

#include "../meta/common_signed_t.hpp"

// adapted from https://stackoverflow.com/a/63436491

namespace uitsl {

template <typename Dividend, typename Divisor>
constexpr uitsl::common_signed_t<Dividend, Divisor>
div_ceil(Dividend x, Divisor y)
{
    if constexpr (std::is_unsigned_v<Dividend> && std::is_unsigned_v<Divisor>) {
        // quotient is always positive
        return x / y + (x % y != 0);  // uint / uint
    }
    else if constexpr (std::is_signed_v<Dividend> && std::is_unsigned_v<Divisor>) {
        auto sy = static_cast<std::make_signed_t<Divisor>>(y);
        bool quotientPositive = x >= 0;
        return x / sy + (x % sy != 0 && quotientPositive);  // int / uint
    }
    else if constexpr (std::is_unsigned_v<Dividend> && std::is_signed_v<Divisor>) {
        auto sx = static_cast<std::make_signed_t<Dividend>>(x);
        bool quotientPositive = y >= 0;
        return sx / y + (sx % y != 0 && quotientPositive);  // uint / int
    }
    else {
        bool quotientPositive = (y >= 0) == (x >= 0);
        return x / y + (x % y != 0 && quotientPositive);  // int / int
    }
}

template <typename Dividend, typename Divisor>
constexpr uitsl::common_signed_t<Dividend, Divisor>
div_floor(Dividend x, Divisor y)
{
    if constexpr (std::is_unsigned_v<Dividend> && std::is_unsigned_v<Divisor>) {
        // quotient is never negative
        return x / y;  // uint / uint
    }
    else if constexpr (std::is_signed_v<Dividend> && std::is_unsigned_v<Divisor>) {
        auto sy = static_cast<std::make_signed_t<Divisor>>(y);
        bool quotientNegative = x < 0;
        return x / sy - (x % sy != 0 && quotientNegative);  // int / uint
    }
    else if constexpr (std::is_unsigned_v<Dividend> && std::is_signed_v<Divisor>) {
        auto sx = static_cast<std::make_signed_t<Dividend>>(x);
        bool quotientNegative = y < 0;
        return sx / y - (sx % y != 0 && quotientNegative);  // uint / int
    }
    else {
        bool quotientNegative = (y < 0) != (x < 0);
        return x / y - (x % y != 0 && quotientNegative);  // int / int
    }
}

template <typename Int>
constexpr signed char sgn(Int n)
{
    return (n > Int{0}) - (n < Int{0});
}

template <typename Dividend, typename Divisor>
constexpr uitsl::common_signed_t<Dividend, Divisor>
div_up(Dividend x, Divisor y)
{
    if constexpr (std::is_unsigned_v<Dividend> && std::is_unsigned_v<Divisor>) {
        // sgn is always 1
        return x / y + (x % y != 0);  // uint / uint
    }
    else if constexpr (std::is_signed_v<Dividend> && std::is_unsigned_v<Divisor>) {
        auto sy = static_cast<std::make_signed_t<Divisor>>(y);
        signed char quotientSgn = sgn(x);
        return x / sy + (x % sy != 0) * quotientSgn;  // int / uint
    }
    else if constexpr (std::is_unsigned_v<Dividend> && std::is_signed_v<Divisor>) {
        auto sx = static_cast<std::make_signed_t<Dividend>>(x);
        signed char quotientSgn = sgn(y);
        return sx / y + (sx % y != 0) * quotientSgn;  // uint / int
    }
    else {
        signed char quotientSgn = sgn(x) * sgn(y);
        return x / y + (x % y != 0) * quotientSgn;  // int / int
    }
}

} // namespace uitsl

#endif // #ifndef UITSL_MATH_DIVIDE_UTILS_HPP_INCLUDE
