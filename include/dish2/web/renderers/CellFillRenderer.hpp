#pragma once
#ifndef DISH2_WEB_RENDERERS_CELLFILLRENDERER_HPP_INCLUDE
#define DISH2_WEB_RENDERERS_CELLFILLRENDERER_HPP_INCLUDE

#include "../../../../third-party/Empirical/source/web/Canvas.h"

#include "../util/CanvasMathHelper.hpp"

namespace dish2 {

template<typename ColorMap, typename Getter>
class CellFillRenderer {

  ColorMap color_map;
  Getter getter;

public:

  CellFillRenderer( Getter getter_ )
  : color_map( getter_ )
  , getter( getter_ )
  {}

  void Render( emp::web::Canvas& canvas ) {

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

#endif // #ifndef DISH2_WEB_RENDERERS_CELLFILLRENDERER_HPP_INCLUDE
