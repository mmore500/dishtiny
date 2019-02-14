#pragma once

#include <experimental/optional>

#include "web/Animate.h"
#include "web/Canvas.h"

namespace UI = emp::web;

template <typename T>
class WebArtist {

private:

  UI::Canvas canvas;

  std::function<std::experimental::optional<T>(size_t)> getter;
  std::function<std::string(std::experimental::optional<T>)> renderer;

  std::string line_color;

  Config &cfg;

public:

  WebArtist(
    UI::Document &viewer,
    std::function<std::experimental::optional<T>(size_t)> getter_,
    std::function<std::string(std::experimental::optional<T>)> renderer_,
    Config &cfg_,
    std::string line_color_="black"
  ) : canvas(500,500)
  , getter(getter_)
  , renderer(renderer_)
  , line_color(line_color_)
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

    // Fill out the grid!
    for (size_t i = 0; i < helper.GetLocalSize(); ++i) {

      canvas.Rect(
        helper.GetLocalX(i)*cell_w+offset_x,
        helper.GetLocalY(i)*cell_h+offset_y,
        cell_w,
        cell_h,
        renderer(getter(i)),
        line_color
      );

    }

  }

};
