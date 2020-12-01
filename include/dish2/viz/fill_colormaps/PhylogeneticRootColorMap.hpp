#pragma once
#ifndef DISH2_VIZ_FILL_COLORMAPS_PHYLOGENETICROOTCOLORMAP_HPP_INCLUDE
#define DISH2_VIZ_FILL_COLORMAPS_PHYLOGENETICROOTCOLORMAP_HPP_INCLUDE

#include "../../../../third-party/Empirical/include/emp/math/math.hpp"
#include "../../../../third-party/Empirical/include/emp/web/color_map.hpp"

namespace dish2 {

struct PhylogeneticRootColorMap {

  template<typename... Args>
  PhylogeneticRootColorMap( Args&&... ){}

  template<typename ValueType>
  std::string Paint(const ValueType& val) const {
    if ( val == 1 ) return "red";
    else if ( val == 2 ) return "green";
    else if ( val == 3 ) return "blue";
    else {
      const size_t hash = emp::hash_combine(val, val);
      return emp::ColorHSV(
        emp::Mod(hash, 360.0),
        emp::Mod(hash, 0.6)+0.4,
        1.0
      );
    }
  }

  void Refresh() { ; }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_FILL_COLORMAPS_PHYLOGENETICROOTCOLORMAP_HPP_INCLUDE
