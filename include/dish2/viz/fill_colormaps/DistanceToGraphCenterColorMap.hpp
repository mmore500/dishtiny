#pragma once
#ifndef DISH2_VIZ_FILL_COLORMAPS_DISTANCETOGRAPHCENTERCOLORMAP_HPP_INCLUDE
#define DISH2_VIZ_FILL_COLORMAPS_DISTANCETOGRAPHCENTERCOLORMAP_HPP_INCLUDE

#include "../../../../third-party/Empirical/include/emp/math/math.hpp"
#include "../../../../third-party/Empirical/include/emp/web/color_map.hpp"

namespace dish2 {

struct DistanceToGraphCenterColorMap {

  template<typename... Args>
  DistanceToGraphCenterColorMap( Args&&... ){}

  template<typename ValueType>
  std::string Paint(const ValueType& val) const {

    return val.has_value() ? emp::ColorHSV(
      emp::Mod(*val, 360.0),
      0.5,
      1.0
    ) : "white";

  }

  void Refresh() { ; }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_FILL_COLORMAPS_DISTANCETOGRAPHCENTERCOLORMAP_HPP_INCLUDE
