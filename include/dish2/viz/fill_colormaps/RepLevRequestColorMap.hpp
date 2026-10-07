#pragma once
#ifndef DISH2_VIZ_FILL_COLORMAPS_REPLEVREQUESTCOLORMAP_HPP_INCLUDE
#define DISH2_VIZ_FILL_COLORMAPS_REPLEVREQUESTCOLORMAP_HPP_INCLUDE

#include <iterator>
#include <string>

#include "../../../../third-party/Empirical/include/emp/math/math.hpp"
#include "../../../../third-party/Empirical/include/emp/web/color_map.hpp"

namespace dish2 {

struct RepLevRequestColorMap {

  template<typename... Args>
  RepLevRequestColorMap( Args&&... ){}

  template<typename ValueType>
  std::string Paint(const ValueType& val) const {
    const size_t rep_lev = val.GetRepLev();
    // no request is set
    if ( rep_lev == std::size( val.GetBuffer() ) ) return "white";
    return emp::ColorHSV(
      emp::Mod( rep_lev * 90.0, 360.0 ),
      1.0,
      1.0
    );
  }

  void Refresh() { ; }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_FILL_COLORMAPS_REPLEVREQUESTCOLORMAP_HPP_INCLUDE
