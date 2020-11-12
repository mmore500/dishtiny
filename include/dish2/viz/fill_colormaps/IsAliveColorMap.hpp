#pragma once
#ifndef DISH2_VIZ_FILL_COLORMAPS_ISALIVECOLORMAP_HPP_INCLUDE
#define DISH2_VIZ_FILL_COLORMAPS_ISALIVECOLORMAP_HPP_INCLUDE

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

  void Refresh() { ; }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_FILL_COLORMAPS_ISALIVECOLORMAP_HPP_INCLUDE
