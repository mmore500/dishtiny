#pragma once
#ifndef DISH2_WEB_FILL_COLORMAPS_RESOURCESTOCKPILECOLORMAP_HPP_INCLUDE
#define DISH2_WEB_FILL_COLORMAPS_RESOURCESTOCKPILECOLORMAP_HPP_INCLUDE

#include "../../../../third-party/Empirical/source/tools/math.h"
#include "../../../../third-party/Empirical/source/web/color_map.h"

namespace dish2 {

struct IsAliveColorMap {

  template<typename... Args>
  IsAliveColorMap( Args&&... ){}

  template<typename ValueType>
  std::string Paint(const ValueType& leaf) const {
    const auto val = leaf.Get();
    if (val) return "transparent";
    else return "black";
  }

};

} // namespace dish2

#endif // #ifndef DISH2_WEB_FILL_COLORMAPS_RESOURCESTOCKPILECOLORMAP_HPP_INCLUDE
