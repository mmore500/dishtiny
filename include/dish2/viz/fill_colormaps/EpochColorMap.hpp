#pragma once
#ifndef DISH2_VIZ_FILL_COLORMAPS_EPOCHCOLORMAP_HPP_INCLUDE
#define DISH2_VIZ_FILL_COLORMAPS_EPOCHCOLORMAP_HPP_INCLUDE

#include "../../../../third-party/Empirical/include/emp/math/math.hpp"
#include "../../../../third-party/Empirical/include/emp/web/color_map.hpp"

namespace dish2 {

struct EpochColorMap {

  template<typename... Args>
  EpochColorMap( Args&&... ){}

  template<typename ValueType>
  std::string Paint(const ValueType& raw) const {
    const size_t val = raw.Get();

    return emp::ColorHSV(
      emp::Mod(val, 360.0),
      0.5,
      1.0
    );

  }

  void Refresh() { ; }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_FILL_COLORMAPS_EPOCHCOLORMAP_HPP_INCLUDE
