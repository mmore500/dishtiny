#pragma once
#ifndef DISH2_VIZ_FILL_COLORMAPS_RESOURCESTOCKPILECOLORMAP_HPP_INCLUDE
#define DISH2_VIZ_FILL_COLORMAPS_RESOURCESTOCKPILECOLORMAP_HPP_INCLUDE

#include <cmath>

#include "../../../../third-party/Empirical/include/emp/math/math.hpp"
#include "../../../../third-party/Empirical/include/emp/web/color_map.hpp"

namespace dish2 {

struct ResourceStockpileColorMap {

  template<typename... Args>
  ResourceStockpileColorMap( Args&&... ){}

  template<typename ValueType>
  std::string Paint(const ValueType& leaf) const {
    const auto val = leaf.Get();
    if ( std::isnan(val) ) return "purple";
    else if (val > 1.0) return emp::ColorRGB(
        std::min(255.0, (val - 1.0) * 25),
        255,
        0
      );
    else if (val > 0) return emp::ColorHSV(
      240,
      (val / 1.0),
      1.0
    );
    else if (val == 0) return "white";
    else return emp::ColorRGB(
      255,
      255 - std::min(255.0, val * 255.0 / -1.0),
      255 - std::min(255.0, val * 255.0 / -1.0)
    );
  }

  void Refresh() { ; }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_FILL_COLORMAPS_RESOURCESTOCKPILECOLORMAP_HPP_INCLUDE
