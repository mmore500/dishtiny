#pragma once
#ifndef DISH2_VIZ_FILL_COLORMAPS_CELLBIRTHCOLORMAP_HPP_INCLUDE
#define DISH2_VIZ_FILL_COLORMAPS_CELLBIRTHCOLORMAP_HPP_INCLUDE

#include "../../../../third-party/Empirical/source/tools/math.h"
#include "../../../../third-party/Empirical/source/web/color_map.h"

namespace dish2 {

struct CellBirthColorMap {

  template<typename... Args>
  CellBirthColorMap( Args&&... ){}

  template<typename ValueType>
  std::string Paint(const ValueType& leaf) const {
    const auto val = leaf.Get();
    if (val == 0) return "green";
    else return "white";
  }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_FILL_COLORMAPS_CELLBIRTHCOLORMAP_HPP_INCLUDE
