#pragma once
#ifndef DISH2_VIZ_FILL_COLORMAPS_BOOLEANCOLORMAP_HPP_INCLUDE
#define DISH2_VIZ_FILL_COLORMAPS_BOOLEANCOLORMAP_HPP_INCLUDE

#include "../../../../third-party/Empirical/include/emp/math/math.hpp"
#include "../../../../third-party/Empirical/include/emp/web/color_map.hpp"

namespace dish2 {

struct BooleanColorMap {

  template<typename... Args>
  BooleanColorMap( Args&&... ){}

  template<typename ValueType>
  std::string Paint(const ValueType val) const {
    if ( val ) return "green";
    else return "white";
  }

  void Refresh() { ; }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_FILL_COLORMAPS_BOOLEANCOLORMAP_HPP_INCLUDE
