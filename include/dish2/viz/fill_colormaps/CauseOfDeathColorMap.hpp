#pragma once
#ifndef DISH2_VIZ_FILL_COLORMAPS_CAUSEOFDEATHCOLORMAP_HPP_INCLUDE
#define DISH2_VIZ_FILL_COLORMAPS_CAUSEOFDEATHCOLORMAP_HPP_INCLUDE

#include "../../../../third-party/Empirical/include/emp/base/always_assert.hpp"
#include "../../../../third-party/Empirical/include/emp/math/math.hpp"
#include "../../../../third-party/Empirical/include/emp/web/color_map.hpp"

#include "../../enum/CauseOfDeath.hpp"

namespace dish2 {

struct CauseOfDeathColorMap {

  template<typename... Args>
  CauseOfDeathColorMap( Args&&... ){}

  template<typename ValueType>
  std::string Paint(const ValueType& val) const {

    if ( !val.has_value() ) return "white";
    else if ( *val == dish2::CauseOfDeath::age ) return "red";
    else if ( *val == dish2::CauseOfDeath::apoptosis ) return "green";
    else if ( *val == dish2::CauseOfDeath::elimination ) return "blue";
    else if ( *val == dish2::CauseOfDeath::fragmentation ) return "yellow";
    else if ( *val == dish2::CauseOfDeath::quorum ) return "purple";
    else {
      emp_always_assert( false, *val );
      __builtin_unreachable();
    }

  }

  void Refresh() { ; }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_FILL_COLORMAPS_CAUSEOFDEATHCOLORMAP_HPP_INCLUDE
