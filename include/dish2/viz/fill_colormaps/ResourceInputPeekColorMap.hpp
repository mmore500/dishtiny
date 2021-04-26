#pragma once
#ifndef DISH2_VIZ_FILL_COLORMAPS_RESOURCEINPUTPEEKCOLORMAP_HPP_INCLUDE
#define DISH2_VIZ_FILL_COLORMAPS_RESOURCEINPUTPEEKCOLORMAP_HPP_INCLUDE

#include "../../../../third-party/Empirical/include/emp/math/math.hpp"
#include "../../../../third-party/Empirical/include/emp/web/color_map.hpp"

namespace dish2 {

struct ResourceInputPeekColorMap {

  template<typename... Args>
  ResourceInputPeekColorMap( Args&&... ){}

  template<typename ValueType>
  std::string Paint(const ValueType& amt) const {
    if (amt > 1.0) return "yellow";
    else if (amt > 0.0) return emp::ColorHSV(
      240.0 - 180.0 * (amt),
      1.0,
      1.0
    );
    else if (amt == 0.0) return "white";
    else return "red";
  }

  void Refresh() { ; }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_FILL_COLORMAPS_RESOURCEINPUTPEEKCOLORMAP_HPP_INCLUDE
