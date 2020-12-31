#pragma once
#ifndef DISH2_VIZ_RENDERERS_CELLBORDERRENDERER_HPP_INCLUDE
#define DISH2_VIZ_RENDERERS_CELLBORDERRENDERER_HPP_INCLUDE

#include <utility>

#include "../../../../third-party/Empirical/include/emp/web/Canvas.hpp"
#include "../../../../third-party/Empirical/include/emp/web/color_map.hpp"

#include "../util/CanvasMathHelper.hpp"

namespace dish2 {

template<typename ColorMap, typename Getter>
class CellBorderRenderer {

  ColorMap color_map;
  Getter getter;

public:

  using getter_t = Getter;

  template<typename... Args>
  CellBorderRenderer( Args&&... args )
  : color_map( std::forward<Args>(args)... )
  , getter( std::forward<Args>(args)... )
  {}

  void Render( emp::web::Canvas& canvas ) {

    const dish2::CanvasMathHelper helper{ canvas };

    for (size_t pos = 0; pos < getter.GetNumCells(); ++pos) {

      canvas.Rect(
        helper.GetCanvasX( helper.GetGridX( pos ) + 1 ),
        helper.GetCanvasY( helper.GetGridY( pos ) ),
        0,
        helper.GetCellWidthPx(),
        emp::ColorRGB(0,0,0,0),
        color_map.Paint(
          getter.Get( pos ),
          getter.Get( helper.GetPopulationPos(
            helper.GetGridX( pos ) + 1,
            helper.GetGridY( pos )
          ) )
        )
      );

      canvas.Rect(
        helper.GetCanvasX( helper.GetGridX( pos ) ),
        helper.GetCanvasY( helper.GetGridY( pos ) + 1 ),
        helper.GetCellHeightPx(),
        0,
        emp::ColorRGB(0,0,0,0),
        color_map.Paint(
          getter.Get( pos ),
          getter.Get( helper.GetPopulationPos(
            helper.GetGridX( pos ),
            helper.GetGridY( pos ) + 1
          ) )
        )
      );

    }

  }


};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_RENDERERS_CELLBORDERRENDERER_HPP_INCLUDE
