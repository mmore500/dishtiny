#pragma once
#ifndef DISH2_VIZ_FILL_COLORMAPS_REGULATIONCOLORMAP_HPP_INCLUDE
#define DISH2_VIZ_FILL_COLORMAPS_REGULATIONCOLORMAP_HPP_INCLUDE

#include <cmath>
#include <limits>

#include "../../../../third-party/conduit/include/uitsl/algorithm/nan_to_zero.hpp"
#include "../../../../third-party/Empirical/include/emp/math/math.hpp"
#include "../../../../third-party/Empirical/include/emp/web/color_map.hpp"

namespace dish2 {

struct RegulationColorMap {

  template<typename... Args>
  RegulationColorMap( Args&&... ){}

  template<typename ValueType>
  std::string Paint(const ValueType& amt) const {

    if ( std::isnan( amt ) ) return "blue";
    else if (amt > 0.0f) return "red";
    else if (amt < 0.0f) return "green";
    else return "white";

  }

  void Refresh() { ; }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_FILL_COLORMAPS_REGULATIONCOLORMAP_HPP_INCLUDE
