#pragma once
#ifndef DISH2_VIZ_RENDERERS_CELLFILLRENDERER_HPP_INCLUDE
#define DISH2_VIZ_RENDERERS_CELLFILLRENDERER_HPP_INCLUDE

#include <utility>

#include "../../../../third-party/Empirical/include/emp/web/Canvas.hpp"
#include "../../../../third-party/Empirical/include/emp/web/color_map.hpp"

#include "../util/CanvasMathHelper.hpp"

namespace dish2 {

template<typename ColorMap, typename Getter>
class CellFillRenderer {

  ColorMap color_map;
  Getter getter;

public:

  using getter_t = Getter;

  template<typename... Args>
  CellFillRenderer( Args&&... args )
  : color_map( std::forward<Args>(args)... )
  , getter( std::forward<Args>(args)... )
  {}

  void Render( emp::web::Canvas& canvas ) {

    color_map.Refresh();

    const dish2::CanvasMathHelper helper{ canvas };

    // Fill out the grid!
    for (size_t pos = 0; pos < getter.GetNumCells(); ++pos) canvas.Rect(
      helper.GetCanvasX( helper.GetGridX( pos ) ),
      helper.GetCanvasY( helper.GetGridY( pos ) ),
      helper.GetCellWidthPx(),
      helper.GetCellHeightPx(),
      color_map.Paint( getter.Get( pos ) ),
      color_map.Paint( getter.Get( pos ) )
    );

  }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_RENDERERS_CELLFILLRENDERER_HPP_INCLUDE
