#pragma once
#ifndef DISH2_VIZ_FILL_COLORMAPS_KINGROUPIDLEVFILLCOLORMAP_HPP_INCLUDE
#define DISH2_VIZ_FILL_COLORMAPS_KINGROUPIDLEVFILLCOLORMAP_HPP_INCLUDE

#include "../../../../third-party/Empirical/include/emp/math/math.hpp"
#include "../../../../third-party/Empirical/include/emp/web/color_map.hpp"

namespace dish2 {

struct KinGroupIDLevFillColorMap {

  template<typename... Args>
  KinGroupIDLevFillColorMap( Args&&... ){}

  template<typename ValueType>
  std::string Paint(const ValueType& val) const {
    return emp::ColorHSV(
      emp::Mod(val, 360.0),
      emp::Mod(val, 0.6) + 0.4,
      1.0
    );

  }

  void Refresh() { ; }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_FILL_COLORMAPS_KINGROUPIDLEVFILLCOLORMAP_HPP_INCLUDE
