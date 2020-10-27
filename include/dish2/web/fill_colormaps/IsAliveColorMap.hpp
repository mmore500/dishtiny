#pragma once
#ifndef DISH2_WEB_FILL_COLORMAPS_RESOURCESTOCKPILECOLORMAP_HPP_INCLUDE
#define DISH2_WEB_FILL_COLORMAPS_RESOURCESTOCKPILECOLORMAP_HPP_INCLUDE

#include "../../../../third-party/Empirical/source/tools/math.h"
#include "../../../../third-party/Empirical/source/web/color_map.h"

namespace dish2 {

template<typename Getter>
struct IsAliveColorMap {

  using value_type = typename Getter::value_type;

  IsAliveColorMap(Getter&){}

  std::string Paint(const value_type& leaf) const {
    const auto val = leaf.Get();
    if (val) return "transparent";
    else return "black";
  }

};

} // namespace dish2

#endif // #ifndef DISH2_WEB_FILL_COLORMAPS_RESOURCESTOCKPILECOLORMAP_HPP_INCLUDE
