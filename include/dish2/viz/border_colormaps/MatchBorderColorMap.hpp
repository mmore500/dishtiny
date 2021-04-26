#pragma once
#ifndef DISH2_VIZ_BORDER_COLORMAPS_MATCHBORDERCOLORMAP_HPP_INCLUDE
#define DISH2_VIZ_BORDER_COLORMAPS_MATCHBORDERCOLORMAP_HPP_INCLUDE

#include <string>
#include <utility>

#include "../../../../third-party/Empirical/include/emp/math/math.hpp"
#include "../../../../third-party/Empirical/include/emp/web/color_map.hpp"

namespace dish2 {

struct MatchBorderColorMap {

  template<typename... Args>
  MatchBorderColorMap( Args&&... ){}

  template<typename ValueType>
  std::string Paint(const ValueType& v1, const ValueType& v2) const {
    if (v1 == v2) return "transparent";
    else return "black";
  }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_BORDER_COLORMAPS_MATCHBORDERCOLORMAP_HPP_INCLUDE
