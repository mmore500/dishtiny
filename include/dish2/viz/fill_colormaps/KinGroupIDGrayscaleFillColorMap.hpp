#pragma once
#ifndef DISH2_VIZ_FILL_COLORMAPS_KINGROUPIDGRAYSCALEFILLCOLORMAP_HPP_INCLUDE
#define DISH2_VIZ_FILL_COLORMAPS_KINGROUPIDGRAYSCALEFILLCOLORMAP_HPP_INCLUDE

#include "../../../../third-party/Empirical/include/emp/math/math.hpp"
#include "../../../../third-party/Empirical/include/emp/web/color_map.hpp"

namespace dish2 {

struct KinGroupIDGrayscaleFillColorMap {

  template<typename... Args>
  KinGroupIDGrayscaleFillColorMap( Args&&... ){}

  template<typename ValueType>
  std::string Paint(const ValueType& in) const {
    const auto& buff = in.GetBuffer();

    return emp::ColorHSV(
      0.0,
      0.0,
      emp::Mod( buff.back(), 0.4 ) + 0.6
    );

  }

  void Refresh() { ; }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_FILL_COLORMAPS_KINGROUPIDGRAYSCALEFILLCOLORMAP_HPP_INCLUDE
