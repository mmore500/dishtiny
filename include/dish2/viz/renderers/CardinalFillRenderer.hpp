#pragma once
#ifndef DISH2_VIZ_RENDERERS_CARDINALFILLRENDERER_HPP_INCLUDE
#define DISH2_VIZ_RENDERERS_CARDINALFILLRENDERER_HPP_INCLUDE

#include <utility>

#include "../../../../third-party/Empirical/include/emp/web/Canvas.hpp"

#include "../util/CanvasMathHelper.hpp"

namespace dish2 {

template<typename ColorMap, typename Getter>
class CardinalFillRenderer {

  ColorMap color_map;
  Getter getter;

public:

  using getter_t = Getter;

  template<typename... Args>
  CardinalFillRenderer( Args&&... args )
  : color_map( std::forward<Args>(args)... )
  , getter( std::forward<Args>(args)... )
  {}

  void Render( emp::web::Canvas& canvas ) {

    color_map.Refresh();

    const dish2::CanvasMathHelper helper{ canvas };

    // draw the background color for each subgrid
    for (size_t pos{}; pos < getter.GetNumCells(); ++pos) {
      for (size_t dir{}; dir < getter.GetNumCardinals( pos ); ++dir) {
        // create a new polygon centered in the middle
        emp::web::CanvasPolygon poly(
          0,
          0,
          color_map.Paint( getter.Get(pos, dir) ),
          color_map.Paint( getter.Get(pos, dir) )
        );
        // add central vertex
        poly.AddPoint(
          helper.GetCanvasX( helper.GetGridX( pos ) + 0.5 ),
          helper.GetCanvasY( helper.GetGridY( pos ) + 0.5 )
        );

        // pick other two vertices depending on direction
        switch ( dir ) {

          case 0: // down triangle
            poly.AddPoint(
              helper.GetCanvasX( helper.GetGridX( pos ) ),
              helper.GetCanvasY( helper.GetGridY( pos ) + 1)
            );
            poly.AddPoint(
              helper.GetCanvasX( helper.GetGridX( pos ) + 1),
              helper.GetCanvasY( helper.GetGridY( pos ) + 1)
            );
            break;

          case 1: // right triangle
            poly.AddPoint(
              helper.GetCanvasX( helper.GetGridX( pos ) + 1),
              helper.GetCanvasY( helper.GetGridY( pos ) + 1)
            );
            poly.AddPoint(
              helper.GetCanvasX( helper.GetGridX( pos ) + 1),
              helper.GetCanvasY( helper.GetGridY( pos ) )
            );
            break;

          case 2: // left triangle
            poly.AddPoint(
              helper.GetCanvasX( helper.GetGridX( pos ) ),
              helper.GetCanvasY( helper.GetGridY( pos ) )
            );
            poly.AddPoint(
              helper.GetCanvasX( helper.GetGridX( pos ) ),
              helper.GetCanvasY( helper.GetGridY( pos ) + 1)
            );
            break;

          case 3: // up triangle
            poly.AddPoint(
              helper.GetCanvasX( helper.GetGridX( pos ) + 1),
              helper.GetCanvasY( helper.GetGridY( pos ) )
            );
            poly.AddPoint(
              helper.GetCanvasX( helper.GetGridX( pos ) ),
              helper.GetCanvasY( helper.GetGridY( pos ) )
            );
            break;

          default:
            emp_assert( false, dir );
            __builtin_unreachable();

        } // dir switch

        // draw the polygon
        canvas.Draw(poly);

      } // dir loop

    } // for loop

  }

};

} // namespace dish2

#endif // #ifndef DISH2_VIZ_RENDERERS_CARDINALFILLRENDERER_HPP_INCLUDE
