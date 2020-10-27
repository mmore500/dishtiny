#pragma once
#ifndef DISH2_WEB_FILL_COLORMAPS_RESOURCESTOCKPILECOLORMAP_HPP_INCLUDE
#define DISH2_WEB_FILL_COLORMAPS_RESOURCESTOCKPILECOLORMAP_HPP_INCLUDE

#include "../../../../third-party/Empirical/source/tools/math.h"
#include "../../../../third-party/Empirical/source/web/color_map.h"

namespace dish2 {

template<typename Getter>
struct ResourceStockpileColorMap {

  using value_type = typename Getter::value_type;

  ResourceStockpileColorMap(Getter&){}

  std::string Paint(const value_type& leaf) const {
    const auto val = leaf.Get();
    if (val > 1.0) return emp::ColorRGB(
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

};

} // namespace dish2

#endif // #ifndef DISH2_WEB_FILL_COLORMAPS_RESOURCESTOCKPILECOLORMAP_HPP_INCLUDE
