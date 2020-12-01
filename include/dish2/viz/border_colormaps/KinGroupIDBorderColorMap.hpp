#pragma once
#ifndef DISH2_VIZ_BORDER_COLORMAPS_KINGROUPIDBORDERCOLORMAP_HPP_INCLUDE
#define DISH2_VIZ_BORDER_COLORMAPS_KINGROUPIDBORDERCOLORMAP_HPP_INCLUDE

#include <string>
#include <utility>

#include "../../../../third-party/Empirical/include/emp/math/math.hpp"
#include "../../../../third-party/Empirical/include/emp/web/color_map.hpp"

namespace dish2 {

struct KinGroupIDBorderColorMap {

  template<typename... Args>
  KinGroupIDBorderColorMap( Args&&... ){}

  template<typename ValueType>
  std::string Paint(const ValueType& in1, const ValueType& in2) const {
    const auto& v1 = in1.data;
    const auto& v2 = in2.data;

    if (v1[0] == v2[0]) return "transparent";
    else if (v1.size() > 1 && v1[1] == v2[1]) return "white";
    else return "black";
  }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_BORDER_COLORMAPS_KINGROUPIDBORDERCOLORMAP_HPP_INCLUDE
