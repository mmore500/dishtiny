#pragma once
#ifndef DISH2_WEB_FILL_COLORMAPS_DUMMYFILLCOLORMAP_HPP_INCLUDE
#define DISH2_WEB_FILL_COLORMAPS_DUMMYFILLCOLORMAP_HPP_INCLUDE

#include <string>

#include "../../../../third-party/Empirical/source/tools/math.h"
#include "../../../../third-party/Empirical/source/web/color_map.h"

namespace dish2 {

template<typename Getter, size_t R=0, size_t G=0, size_t B=0>
struct DummyFillColorMap {

  using value_type = typename Getter::value_type;

  DummyFillColorMap(Getter){}

  std::string Paint(const value_type&) const {
    return emp::ColorRGB( R, G, B );
  }

};

} // namespace dish2

#endif // #ifndef DISH2_WEB_FILL_COLORMAPS_DUMMYFILLCOLORMAP_HPP_INCLUDE
