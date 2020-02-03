#pragma once

#include <optional>
#include <limits>

#include "web/Animate.h"
#include "web/Canvas.h"
#include "web/DocuExtras.h"
#include "web/color_map.h"

#include "WebArtistBase.h"

class WebArtistConnection : public WebArtistBase {

private:

  const std::string name;

  UI::Canvas canvas;
  UI::DocuExtras description;

  std::function<emp::vector<size_t>(size_t)> getter;

  const Config &cfg;

  size_t last_update = 0;

public:

  WebArtistConnection(
    std::string name_,
    std::string description_,
    UI::Document &viewer,
    std::function<emp::vector<size_t>(size_t)> getter_,
    const Config &cfg_
  ) : name(
    name_
  ), canvas(
    std::min(GetViewPortSize() - 100, 500),
    std::min(GetViewPortSize() - 100, 500)
  ), description(
    emp::to_string(emp::slugify(description_), "-key")
  ), getter(
    getter_
  ), cfg(
    cfg_
  ) { viewer << canvas.SetCSS(
      "position", "absolute",
      "margin-left", "auto",
      "margin-right", "auto",
      "left", "0",
      "right", "0"
    );
  }

  void Deactivate() {
    canvas.SetCSS("visibility", "hidden");
    description.SetCSS("display", "none");
  }

  void Activate() {
    canvas.SetCSS("visibility", "visible");
    description.SetCSS("display", "initial");
  }

  void Download(const std::string & fn) { canvas.DownloadPNG(fn); }

  std::string GetName() const { return name; }

  void Redraw(const size_t update) {

    if (update == last_update || canvas.GetCSS("visibility") == "hidden") {
      return;
    }
    else last_update = update;

    // Determine the canvas info.
    const double canvas_w = canvas.GetWidth();
    const double canvas_h = canvas.GetHeight();

    // Determine the cell width & height.
    const double cell_w = canvas_w /  cfg.GRID_W();
    const double cell_h = canvas_h / cfg.GRID_H();

    // Determine the realized grid width and height on the canvas.
    const double grid_w = cell_w * cfg.GRID_W();
    const double grid_h = cell_h * cfg.GRID_H();

    // Center the grid on the canvas if there's extra room.
    const double offset_x = (
      (canvas_w <= grid_w)
      ? 0
      : (canvas_w - grid_w) / 2
    ) + cell_w / 2;
    const double offset_y = (
      (canvas_h <= grid_h)
      ? 0
      : (canvas_h - grid_h) / 2
    ) + cell_h / 2;

    canvas.Clear();

    // Setup a black background for the grid.
    canvas.Rect(
      0,
      0,
      canvas.GetWidth(),
      canvas.GetHeight(),
      emp::ColorRGB(255,255,255)
    );

    GeometryHelper helper(cfg);

    const auto GridXToCanvasX = [cell_w, offset_x](const double grid_x){
      return grid_x*cell_w+offset_x;
    };
    const auto GridYToCanvasY = [cell_h, offset_y](const double grid_y){
      return grid_y*cell_h+offset_y;
    };

    // Fill out the grid!
    for (size_t from = 0; from < helper.GetLocalSize(); ++from) {
      for (const auto &to : getter(from)) {

        const double to_x = helper.GetLocalX(to);
        const double to_y = helper.GetLocalY(to);

        const double from_x = helper.GetLocalX(from);
        const double from_y = helper.GetLocalY(from);

        const double wraparound_x = (
          std::abs(to_x - from_x) > (cfg.GRID_W() / 2.0)
          ? (
            from_x < (cfg.GRID_W() / 2.0)
            ? -static_cast<double>(cfg.GRID_W())
            : static_cast<double>(cfg.GRID_W())
          ) : 0.0
        );

        const double wraparound_y = (
          std::abs(to_y - from_y) > (cfg.GRID_H() / 2.0)
          ? (
            from_y < (cfg.GRID_H() / 2.0)
            ? -static_cast<double>(cfg.GRID_H())
            : static_cast<double>(cfg.GRID_H())
          ) : 0.0
        );

        canvas.Line(
          GridXToCanvasX(from_x),
          GridYToCanvasY(from_y),
          GridXToCanvasX(to_x + wraparound_x),
          GridYToCanvasY(to_y + wraparound_y),
          "black"
        );
        if (wraparound_x || wraparound_y) {
          canvas.Line(
            GridXToCanvasX(from_x - wraparound_x),
            GridYToCanvasY(from_y - wraparound_y),
            GridXToCanvasX(to_x),
            GridYToCanvasY(to_y),
            "black"
          );
        }

        canvas.Circle(
          GridXToCanvasX(from_x),
          GridYToCanvasY(from_y),
          1.0,
          "red",
          "red"
        );
        canvas.Circle(
          GridXToCanvasX(to_x),
          GridYToCanvasY(to_y),
          1.0,
          "green",
          "green"
        );
      }
    }

  }


};
