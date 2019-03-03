#pragma once

#include <optional>

#include "web/Animate.h"
#include "web/Canvas.h"
#include "web/color_map.h"

namespace UI = emp::web;

template <typename T>
class WebArtist {

private:

  UI::Canvas canvas;

  std::function<std::optional<T>(size_t)> getter;
  std::function<std::string(std::optional<T>)> renderer;

  std::function<std::string(std::optional<T>,std::optional<T>)> divider;

  const Config &cfg;

public:

  WebArtist(
    UI::Document &viewer,
    std::function<std::optional<T>(size_t)> getter_,
    std::function<std::string(std::optional<T>)> renderer_,
    const Config &cfg_,
    std::function<std::string(std::optional<T>,std::optional<T>)> divider_=[](std::optional<T>,std::optional<T>){ return "gray"; }
  ) : canvas(500,500)
  , getter(getter_)
  , renderer(renderer_)
  , divider(divider_)
  , cfg(cfg_)
  { viewer << canvas; }

  void Redraw() {

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
      emp::ColorRGB(0,0,0)
    );

    GeometryHelper helper(cfg);

    const auto GridXToCanvasX = [cell_w, offset_x](size_t grid_x){
      return grid_x*cell_w+offset_x;
    };
    const auto GridYToCanvasY = [cell_h, offset_y](size_t grid_y){
      return grid_y*cell_h+offset_y;
    };


    // Fill out the grid!
    for (size_t i = 0; i < helper.GetLocalSize(); ++i) {

      canvas.Rect(
        GridXToCanvasX(helper.GetLocalX(i)),
        GridYToCanvasY(helper.GetLocalY(i)),
        cell_w,
        cell_h,
        renderer(getter(i)),
        renderer(getter(i))
      );

    }

    for (size_t i = 0; i < helper.GetLocalSize(); ++i) {

        canvas.Rect(
          GridXToCanvasX(helper.GetLocalX(i)+1),
          GridYToCanvasY(helper.GetLocalY(i)),
          0,
          cell_h,
          emp::ColorRGB(0,0,0,0),
          divider(
            getter(i),
            getter(helper.GetLocalPos(
              helper.GetLocalX(i)+1,
              helper.GetLocalY(i)
            ))
          )
        );
        canvas.Rect(
          GridXToCanvasX(helper.GetLocalX(i)),
          GridYToCanvasY(helper.GetLocalY(i)+1),
          cell_w,
          0,
          emp::ColorRGB(0,0,0,0),
          divider(
            getter(i),
            getter(helper.GetLocalPos(
              helper.GetLocalX(i),
              helper.GetLocalY(i)+1
            ))
          )
        );

    }

  }

  void Download(const std::string & fn) { canvas.DownloadPNG(fn); }

};
