#pragma once
#ifndef DISH2_VIZ_FILL_COLORMAPS_KINGROUPAGECOLORMAP_HPP_INCLUDE
#define DISH2_VIZ_FILL_COLORMAPS_KINGROUPAGECOLORMAP_HPP_INCLUDE

#include "../../../../third-party/Empirical/include/emp/math/math.hpp"
#include "../../../../third-party/Empirical/include/emp/web/color_map.hpp"

namespace dish2 {

struct KinGroupAgeColorMap {

  template<typename... Args>
  KinGroupAgeColorMap( Args&&... ){}

  template<typename ValueType>
  std::string Paint(const ValueType& in) const {

    return emp::ColorHSV(
      emp::Mod(in, 360.0),
      0.5,
      1.0
    );

  }

  void Refresh() { ; }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_FILL_COLORMAPS_KINGROUPAGECOLORMAP_HPP_INCLUDE
