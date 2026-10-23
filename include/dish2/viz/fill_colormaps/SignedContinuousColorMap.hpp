#pragma once
#ifndef DISH2_VIZ_FILL_COLORMAPS_SIGNEDCONTINUOUSCOLORMAP_HPP_INCLUDE
#define DISH2_VIZ_FILL_COLORMAPS_SIGNEDCONTINUOUSCOLORMAP_HPP_INCLUDE

#include <cmath>
#include <string>

#include "../../../../third-party/Empirical/include/emp/math/math.hpp"
#include "../../../../third-party/Empirical/include/emp/web/color_map.hpp"

namespace dish2 {

struct SignedContinuousColorMap {

  template<typename... Args>
  SignedContinuousColorMap( Args&&... ){}

  template<typename ValueType>
  std::string Paint(const ValueType& val) const {

    if ( std::isnan( val ) ) return "purple";
    else if ( val == 0 ) return "white";

    // squash magnitude into [0, 1) so that large values remain distinguishable
    const double mag = std::abs( static_cast<double>( val ) );
    const double squashed = std::isinf( mag ) ? 1.0 : mag / (1.0 + mag);

    // transition from pastel (small) to fluorescent (large), with a discrete
    // jump from zero (white) so that small nonzero values are distinguishable
    return emp::ColorHSV(
      val > 0 ? 240 : 0,
      0.2 + 0.8 * squashed,
      1.0
    );

  }

  void Refresh() { ; }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_FILL_COLORMAPS_SIGNEDCONTINUOUSCOLORMAP_HPP_INCLUDE
