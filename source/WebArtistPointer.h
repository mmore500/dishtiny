#pragma once

#include <optional>
#include <limits>

#include "web/Animate.h"
#include "web/Canvas.h"
#include "web/DocuExtras.h"
#include "web/color_map.h"

#include "WebArtistBase.h"

template <typename T>
class WebArtistPointer : public WebArtistBase {

private:

  const std::string name;

  UI::Canvas canvas;
  UI::DocuExtras description;

  std::function<std::optional<T>(size_t, size_t)> getter;
  std::function<std::string(std::optional<T>)> renderer;

  std::function<std::string(std::optional<T>,std::optional<T>)> divider;

  const Config &cfg;

  size_t last_update;

public:

  WebArtistPointer(
    std::string name_,
    std::string description_,
    UI::Document &viewer,
    std::function<std::optional<T>(size_t)> getter_,
    std::function<std::string(std::optional<T>)> renderer_,
    const Config &cfg_,
    std::function<std::string(std::optional<T>,std::optional<T>)> divider_=[](std::optional<T>,std::optional<T>){ return "gray"; }
  ) : name(name_)
  , canvas(
    std::min(GetViewPortSize() - 100, 500),
    std::min(GetViewPortSize() - 100, 500)
  )
  , description(emp::to_string(emp::slugify(description_), "-key"))
  , getter(getter_)
  , renderer(renderer_)
  , divider(divider_)
  , cfg(cfg_)
  , last_update(std::numeric_limits<size_t>::max())
  { viewer << canvas.SetCSS(
      "position", "absolute",
      "margin-left", "auto",
      "margin-right", "auto",
      "left", "0",
      "right", "0"
    );
  }

  // for use as background in WebArtistConnection
  WebArtistPointer(
    std::string name_,
    std::string description_,
    UI::Canvas &canvas_,
    std::function<std::optional<T>(size_t)> getter_,
    std::function<std::string(std::optional<T>)> renderer_,
    const Config &cfg_,
    std::function<std::string(std::optional<T>,std::optional<T>)> divider_=[](std::optional<T>,std::optional<T>){ return "gray"; }
  ) : name(name_)
  , canvas(canvas_)
  , description(emp::to_string(emp::slugify(description_), "-key"))
  , getter(getter_)
  , renderer(renderer_)
  , divider(divider_)
  , cfg(cfg_)
  , last_update(std::numeric_limits<size_t>::max())
  { ; }

  void Deactivate() {
    canvas.SetCSS("visibility", "hidden");
    description.SetCSS("display", "none");
  }

  void Activate() {
    canvas.SetCSS("visibility", "visible");
    description.SetCSS("display", "initial");
  }

  std::string GetName() const { return name; }

  void Redraw(const size_t update) {

    if (update == last_update || canvas.GetCSS("visibility") == "hidden") {
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
      "black"
    );

    GeometryHelper helper(cfg);

    const auto GridXToCanvasX = [cell_w, offset_x](size_t grid_x){
      return grid_x*cell_w+offset_x;
    };
    const auto GridYToCanvasY = [cell_h, offset_y](size_t grid_y){
      return grid_y*cell_h+offset_y;
    };


    /* Fill out the grid! */

    // first we draw the background
    for (size_t i = 0; i < helper.GetLocalSize(); ++i) {
      // create a new rectangle...
      canvas.Rect(
        // ...at pos (x, y)...
        GridXToCanvasX(helper.GetLocalX(i)),
        GridYToCanvasY(helper.GetLocalY(i)),

        // ...with width w and height h...
        cell_w,
        cell_h,

        // ...and the appropiate face and line colors (optional)
        renderer(getter(i)),
        renderer(getter(i))
      );

    }

    // and then we draw the boxes around it
    for (size_t i = 0; i < helper.GetLocalSize(); ++i) {
        // right edge
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
        // bottom edge
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
