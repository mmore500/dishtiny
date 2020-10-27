#pragma once
#ifndef DISH2_WEB_FILL_COLORMAPS_DUMMYFILLCOLORMAP_HPP_INCLUDE
#define DISH2_WEB_FILL_COLORMAPS_DUMMYFILLCOLORMAP_HPP_INCLUDE

#include <string>

#include "../../../../third-party/Empirical/source/tools/math.h"
#include "../../../../third-party/Empirical/source/web/color_map.h"

namespace dish2 {

template<unsigned char R=0, unsigned char G=0, unsigned char B=0>
struct DummyFillColorMap {

  template<typename... Args>
  DummyFillColorMap( Args&&... ){}

  template<typename ValueType>
  std::string Paint(const ValueType&) const {
    return emp::ColorRGB( R, G, B );
  }

};

} // namespace dish2

#endif // #ifndef DISH2_WEB_FILL_COLORMAPS_DUMMYFILLCOLORMAP_HPP_INCLUDE
