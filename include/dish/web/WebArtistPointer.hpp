#pragma once
#ifndef DISH_WEB_WEBARTISTPOINTER_HPP_INCLUDE
#define DISH_WEB_WEBARTISTPOINTER_HPP_INCLUDE

#include <cmath>
#include <limits>
#include <optional>

#include "../../../third-party/Empirical/source/web/Animate.h"
#include "../../../third-party/Empirical/source/web/Canvas.h"
#include "../../../third-party/Empirical/source/web/color_map.h"
#include "../../../third-party/Empirical/source/web/DocuExtras.h"

#include "WebArtistBase.hpp"

#include "../topology/Cardi.hpp"

namespace dish {

template <typename T>
class WebArtistPointer : public WebArtistBase {

private:

  std::function<std::optional<T>(size_t, size_t)> getter;
  std::function<std::string(std::optional<T>)> renderer;

  std::function<std::string(std::optional<T>,std::optional<T>)> divider;

public:

  WebArtistPointer(
    std::string name_,
    std::string description_,
    UI::Document &viewer_,
    std::function<std::optional<T>(size_t, size_t)> getter_,
    std::function<std::string(std::optional<T>)> renderer_,
    const Config &cfg_,
    std::function<std::string(std::optional<T>,std::optional<T>)> divider_=[](std::optional<T>,std::optional<T>){ return "gray"; }
  ) : WebArtistBase(
        name_,
        description_,
        viewer_,
        cfg_
      ),
      getter(getter_),
      renderer(renderer_),
      divider(divider_)
    { ; }
  void Redraw(const size_t update) {

    if (update == last_update || description.GetCSS("display") == "none") {
      return;
    }
    else last_update = update;

    // Determine the canvas info.
    const size_t canvas_w = canvas.GetWidth();
    const size_t canvas_h = canvas.GetHeight();

    // Determine the cell width & height.
    const size_t cell_w = canvas_w /  cfg.GRID_W();
    const size_t cell_h = canvas_h / cfg.GRID_H();

    // Determine the realized grid width and height on the canvas.
    const size_t grid_w = cell_w * cfg.GRID_W();
    const size_t grid_h = cell_h * cfg.GRID_H();

    // Center the grid on the canvas if there's extra room.
    const size_t offset_x = (canvas_w <= grid_w) ? 0 : (canvas_w - grid_w) / 2;
    const size_t offset_y = (canvas_h <= grid_h) ? 0 : (canvas_h - grid_h) / 2;

    canvas.Clear();

    // Setup a black background for the grid.
    canvas.Rect(
      0,
      0,
      canvas.GetWidth(),
      canvas.GetHeight(),
      "white"
    );

    GeometryHelper helper(cfg);

    // for grids
    const auto GridXToCanvasX = [cell_w, offset_x](const double grid_x){
      return grid_x*cell_w+offset_x;
    };
    const auto GridYToCanvasY = [cell_h, offset_y](const double grid_y){
      return grid_y*cell_h+offset_y;
    };


    /* Fill out the grid! */

    // draw the background color for each subgrid
    for (size_t i = 0; i < helper.GetLocalSize(); ++i) {
      for (size_t j = 0; j < Cardi::Dir::NumDirs; ++j) {
        // create a new polygon centered in the middle
        UI::CanvasPolygon poly(
          0,
          0,
          renderer(getter(i, j)),
          renderer(getter(i, j))
        );
        // add central vertex
        poly.AddPoint(
          GridXToCanvasX(helper.GetLocalX(i) + 0.5),
          GridYToCanvasY(helper.GetLocalY(i) + 0.5)
        );
        // pick other two vertices depending on direction
        switch (j) {
          // north triangle
          case 0:
            // down triangle
            poly.AddPoint(
              GridXToCanvasX(helper.GetLocalX(i) + 1),
              GridYToCanvasY(helper.GetLocalY(i) + 1)
            );
            poly.AddPoint(
              GridXToCanvasX(helper.GetLocalX(i)),
              GridYToCanvasY(helper.GetLocalY(i) + 1)
            );
            break;
          case 1:
            // up triangle
            poly.AddPoint(
              GridXToCanvasX(helper.GetLocalX(i)),
              GridYToCanvasY(helper.GetLocalY(i))
            );
            poly.AddPoint(
              GridXToCanvasX(helper.GetLocalX(i) + 1),
              GridYToCanvasY(helper.GetLocalY(i))
            );
            break;
          // west triangle
          case 2:
            // right triangle
            poly.AddPoint(
              GridXToCanvasX(helper.GetLocalX(i) + 1),
              GridYToCanvasY(helper.GetLocalY(i))
            );
            poly.AddPoint(
              GridXToCanvasX(helper.GetLocalX(i) + 1),
              GridYToCanvasY(helper.GetLocalY(i) + 1)
            );
            break;
          // south triangle
          case 3:
            // left triangle
            poly.AddPoint(
              GridXToCanvasX(helper.GetLocalX(i)),
              GridYToCanvasY(helper.GetLocalY(i) + 1)
            );
            poly.AddPoint(
              GridXToCanvasX(helper.GetLocalX(i)),
              GridYToCanvasY(helper.GetLocalY(i))
            );
            break;
        }

        // draw the polygon
        canvas.Draw(poly);
      }
    }
    // and then we draw the boxes around each grid
    for (size_t i = 0; i < helper.GetLocalSize(); ++i) {
        // right edge
        canvas.Rect(
          GridXToCanvasX(helper.GetLocalX(i)+1),
          GridYToCanvasY(helper.GetLocalY(i)),
          0,
          cell_h,
          emp::ColorRGB(0,0,0,0),
          divider(
            getter(i, 0),
            getter(helper.GetLocalPos(
              helper.GetLocalX(i)+1,
              helper.GetLocalY(i)
            ), 0)
          )
        );
        // bottom edge
        canvas.Rect(
          GridXToCanvasX(helper.GetLocalX(i)),
          GridYToCanvasY(helper.GetLocalY(i)+1),
          cell_w,
          0,
          emp::ColorRGB(0,0,0,0),
          divider(
            getter(i, 0),
            getter(helper.GetLocalPos(
              helper.GetLocalX(i),
              helper.GetLocalY(i)+1
            ), 0)
          )
        );
    }

  }

  void Download(const std::string & fn) {
    if (description.GetCSS("display") != "none") canvas.DownloadPNG(fn);
  }

};

} // namespace dish

#endif // #ifndef DISH_WEB_WEBARTISTPOINTER_HPP_INCLUDE
