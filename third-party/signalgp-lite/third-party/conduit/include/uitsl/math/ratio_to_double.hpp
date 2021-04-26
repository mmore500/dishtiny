#pragma once
#ifndef UITSL_MATH_RATIO_TO_DOUBLE_HPP_INCLUDE
#define UITSL_MATH_RATIO_TO_DOUBLE_HPP_INCLUDE

namespace uitsl {

template<typename Ratio>
constexpr double ratio_to_double() {
  return Ratio::num / static_cast<double>( Ratio::den );
}

template<typename Ratio>
constexpr double as_double() { return ratio_to_double<Ratio>(); }

} // namespace uitsl

#endif // #ifndef UITSL_MATH_RATIO_TO_DOUBLE_HPP_INCLUDE
