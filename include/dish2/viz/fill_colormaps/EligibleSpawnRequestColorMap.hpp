#pragma once
#ifndef DISH2_VIZ_FILL_COLORMAPS_ELIGIBLESPAWNREQUESTCOLORMAP_HPP_INCLUDE
#define DISH2_VIZ_FILL_COLORMAPS_ELIGIBLESPAWNREQUESTCOLORMAP_HPP_INCLUDE

#include <string>
#include <tuple>

namespace dish2 {

struct EligibleSpawnRequestColorMap {

  template<typename... Args>
  EligibleSpawnRequestColorMap( Args&&... ){}

  // state is (requested, arrested)
  std::string Paint(const std::tuple<bool, bool>& state) const {
    const auto& [requested, arrested] = state;
    if ( requested && arrested ) return "magenta";
    else if ( requested ) return "blue";
    else if ( arrested ) return "red";
    else return "white";
  }

  void Refresh() { ; }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_FILL_COLORMAPS_ELIGIBLESPAWNREQUESTCOLORMAP_HPP_INCLUDE
