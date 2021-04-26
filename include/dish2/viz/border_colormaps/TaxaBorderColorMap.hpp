#pragma once
#ifndef DISH2_VIZ_BORDER_COLORMAPS_TAXABORDERCOLORMAP_HPP_INCLUDE
#define DISH2_VIZ_BORDER_COLORMAPS_TAXABORDERCOLORMAP_HPP_INCLUDE

#include <string>
#include <tuple>
#include <utility>

#include "../../../../third-party/Empirical/include/emp/math/math.hpp"
#include "../../../../third-party/Empirical/include/emp/web/color_map.hpp"

namespace dish2 {

struct TaxaBorderColorMap {

  template<typename... Args>
  TaxaBorderColorMap( Args&&... ){}

  template<typename ValueType>
  std::string Paint(const ValueType& v1, const ValueType& v2) const {
    if( std::tuple{
      v1.event_tags,
      v1.program
    } != std::tuple{
      v2.event_tags,
      v2.program
    } ) return "red";
    else return "transparent";
  }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_BORDER_COLORMAPS_TAXABORDERCOLORMAP_HPP_INCLUDE
