#pragma once
#ifndef DISH2_VIZ_FILL_COLORMAPS_KINGROUPIDFILLCOLORMAP_HPP_INCLUDE
#define DISH2_VIZ_FILL_COLORMAPS_KINGROUPIDFILLCOLORMAP_HPP_INCLUDE

#include "../../../../third-party/Empirical/include/emp/math/math.hpp"
#include "../../../../third-party/Empirical/include/emp/web/color_map.hpp"

namespace dish2 {

struct KinGroupIDFillColorMap {

  template<typename... Args>
  KinGroupIDFillColorMap( Args&&... ){}

  template<typename ValueType>
  std::string Paint(const ValueType& in) const {
    const auto& val = in.GetBuffer();

    return emp::ColorHSV(
      emp::Mod(val.size() > 1 ? val[1] : val[0], 360.0),
      emp::Mod(val[0], 0.6) + 0.4,
      1.0
    );

  }

  void Refresh() { ; }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_FILL_COLORMAPS_KINGROUPIDFILLCOLORMAP_HPP_INCLUDE
