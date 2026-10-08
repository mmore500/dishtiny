#pragma once
#ifndef DISH2_VIZ_FILL_COLORMAPS_KINMATCHCOLORMAP_HPP_INCLUDE
#define DISH2_VIZ_FILL_COLORMAPS_KINMATCHCOLORMAP_HPP_INCLUDE

#include <algorithm>
#include <iterator>
#include <string>

#include "../../../../third-party/conduit/include/uitsl/polyfill/identity.hpp"
#include "../../../../third-party/Empirical/include/emp/math/math.hpp"
#include "../../../../third-party/Empirical/include/emp/web/color_map.hpp"

namespace dish2 {

struct KinMatchColorMap {

  template<typename... Args>
  KinMatchColorMap( Args&&... ){}

  template<typename ValueType>
  std::string Paint(const ValueType kin_match_array) const {
    if ( std::all_of(
      std::begin( kin_match_array ),
      std::end( kin_match_array ),
      std::identity
    ) ) return "green";
    else return "white";
  }

  void Refresh() { ; }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_FILL_COLORMAPS_KINMATCHCOLORMAP_HPP_INCLUDE
