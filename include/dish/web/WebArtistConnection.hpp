#pragma once
#ifndef DISH_WEBARTISTCONNECTION_HPP_INCLUDE
#define DISH_WEBARTISTCONNECTION_HPP_INCLUDE

#include <optional>
#include <limits>
#include <cmath>

#include "../../../third-party/Empirical/source/web/Animate.h"
#include "../../../third-party/Empirical/source/web/Canvas.h"
#include "../../../third-party/Empirical/source/web/DocuExtras.h"
#include "../../../third-party/Empirical/source/web/color_map.h"

#include "../world/DishWorld.hpp"

#include "WebArtistBase.hpp"
#include "WebArtistCell.hpp"

namespace dish {

std::string ChannelGrayscale(ChannelPack &cp) {
  return emp::ColorHSV(
    0.0,
    0.0,
    emp::Mod(cp.size() > 1 ? cp[1] : cp[0],0.4)+0.6
  );
}

// adapted from https://stackoverflow.com/a/4609795
template <typename T>
int signum(const T val) { return (T(0) < val) - (val < T(0)); }

class WebArtistConnection : public WebArtistBase {

private:

  std::function<emp::vector<size_t>(size_t)> getter;

  WebArtistCell<ChannelPack> background;

public:

  WebArtistConnection(
    std::string name_,
    std::string description_,
    UI::Document &viewer_,
    std::function<emp::vector<size_t>(size_t)> getter_,
    const Config &cfg_,
    const DishWorld &w
  ) : WebArtistBase(
        name_,
        description_,
        viewer_,
        cfg_
      ),
      getter(getter_),
      background(
        "Channel",
        "Channel",
        canvas,
        [&w](const size_t i){
          return w.GetManager().Channel(i).GetIDs();
        },
        [](std::optional<ChannelPack> cp) {
          return cp ? ChannelGrayscale(*cp) : "black";
        },
        cfg_,
        [](std::optional<ChannelPack> cp1, std::optional<ChannelPack> cp2) -> std::string {
          if (!cp1 || !cp2) return "black";
          else if ((*cp1)[0] == (*cp2)[0]) return "transparent";
          else if (cp1->size() > 1 && (*cp1)[1] == (*cp2)[1]) return "lightgray";
          else return "black";
        }
      )
    { ; }


  void Download(const std::string & fn) {
    if (description.GetCSS("display") != "none") canvas.DownloadPNG(fn);
  }

  void Redraw(const size_t update) {

    if (update == last_update || description.GetCSS("display") == "none") {
      return;
    }
    else last_update = update;

    background.Redraw(update);

    // Determine the canvas info.
    const double canvas_w = canvas.GetWidth();
    const double canvas_h = canvas.GetHeight();

    // Determine the cell width & height.
    const double cell_w = std::floor(canvas_w / cfg.GRID_W());
    const double cell_h = std::floor(canvas_h / cfg.GRID_H());

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

        const double from_x = helper.GetLocalX(from);
        const double from_y = helper.GetLocalY(from);

        const double to_x = (
          helper.GetLocalX(to)
          + (
            std::abs(from_x - helper.GetLocalX(to))
            > (cfg.GRID_W() / 2.0)
            ? -1.0 : 1.0
          ) * signum(from_x - helper.GetLocalX(to)) * 0.25
        );
        const double to_y = (
          helper.GetLocalY(to)
          + ( // must do opposite when wrapping around
            std::abs(from_y - helper.GetLocalY(to)) > (cfg.GRID_H() / 2.0)
            ? -1.0 : 1.0
          ) * signum(from_y - helper.GetLocalY(to)) * 0.25
        );

        canvas.Circle(
          GridXToCanvasX(to_x),
          GridYToCanvasY(to_y),
          2,
          "#15f4ee",
          "blueviolet",
          1.5
        );

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
          "blueviolet",
          3.5
        );
        canvas.Line(
          GridXToCanvasX(from_x),
          GridYToCanvasY(from_y),
          GridXToCanvasX(to_x + wraparound_x),
          GridYToCanvasY(to_y + wraparound_y),
          "#15f4ee",
          1.5
        );
        if (wraparound_x || wraparound_y) {
          canvas.Line(
            GridXToCanvasX(from_x - wraparound_x),
            GridYToCanvasY(from_y - wraparound_y),
            GridXToCanvasX(to_x),
            GridYToCanvasY(to_y),
            "blueviolet",
            3.5
          );
          canvas.Line(
            GridXToCanvasX(from_x - wraparound_x),
            GridYToCanvasY(from_y - wraparound_y),
            GridXToCanvasX(to_x),
            GridYToCanvasY(to_y),
            "#15f4ee",
            1.5
          );
        }

      }
    }

  }


};

} // namespace dish

#endif // #ifndef DISH_WEBARTISTCONNECTION_HPP_INCLUDE
