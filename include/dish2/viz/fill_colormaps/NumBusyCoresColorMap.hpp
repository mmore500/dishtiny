#pragma once
#ifndef DISH2_VIZ_FILL_COLORMAPS_NUMBUSYCORESCOLORMAP_HPP_INCLUDE
#define DISH2_VIZ_FILL_COLORMAPS_NUMBUSYCORESCOLORMAP_HPP_INCLUDE

#include "../../../../third-party/Empirical/include/emp/math/math.hpp"
#include "../../../../third-party/Empirical/include/emp/web/color_map.hpp"

namespace dish2 {

struct NumBusyCoresColorMap {

  template<typename... Args>
  NumBusyCoresColorMap( Args&&... ){}

  template<typename ValueType>
  std::string Paint(const ValueType& val) const {
    if (val == 0) return "white";
    else return emp::ColorHSV(
      emp::Mod(val * 20, 360.0),
      0.5,
      1.0
    );
  }

  void Refresh() { ; }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_FILL_COLORMAPS_NUMBUSYCORESCOLORMAP_HPP_INCLUDE
