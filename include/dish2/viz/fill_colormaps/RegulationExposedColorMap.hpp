#pragma once
#ifndef DISH2_VIZ_FILL_COLORMAPS_REGULATIONEXPOSEDCOLORMAP_HPP_INCLUDE
#define DISH2_VIZ_FILL_COLORMAPS_REGULATIONEXPOSEDCOLORMAP_HPP_INCLUDE

#include <cmath>
#include <limits>

#include "../../../../third-party/conduit/include/uitsl/algorithm/nan_to_zero.hpp"
#include "../../../../third-party/Empirical/include/emp/math/math.hpp"
#include "../../../../third-party/Empirical/include/emp/web/color_map.hpp"

namespace dish2 {

struct RegulationExposedColorMap {

  template<typename... Args>
  RegulationExposedColorMap( Args&&... ){}

  template<typename ValueType>
  std::string Paint(const ValueType& opt_amt) const {

    if ( !opt_amt.has_value() ) return "gray";
    if ( std::isnan( *opt_amt ) ) return "blue";
    else if (*opt_amt > 0.0f) return "red";
    else if (*opt_amt < 0.0f) return "green";
    else return "white";

  }

  void Refresh() { ; }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_FILL_COLORMAPS_REGULATIONEXPOSEDCOLORMAP_HPP_INCLUDE
