#pragma once

#include <optional>
#include <limits>
#include <chrono>

#include "web/Animate.h"
#include "web/Canvas.h"
#include "web/DocuExtras.h"
#include "web/color_map.h"

#include "WebArtistBase.h"

template <typename T>
class WebArtistCell : public WebArtistBase {

private:

  const std::string name;

  UI::Canvas canvas;
  UI::Document* viewer;
  UI::DocuExtras description;

  std::function<std::optional<T>(size_t)> getter;
  std::function<std::string(std::optional<T>)> renderer;

  std::function<std::string(std::optional<T>,std::optional<T>)> divider;

  const Config &cfg;

  size_t last_update;

public:

  WebArtistCell(
    std::string name_,
    std::string description_,
    UI::Document & viewer_,
    std::function<std::optional<T>(size_t)> getter_,
    std::function<std::string(std::optional<T>)> renderer_,
    const Config &cfg_,
    std::function<std::string(std::optional<T>,std::optional<T>)> divider_=[](std::optional<T>,std::optional<T>){ return "gray"; }
  ) : name(name_)
  , canvas(
    std::min(GetViewPortSize() - 100, 500),
    std::min(GetViewPortSize() - 100, 500)
  )
  , viewer(&viewer_)
  , description(emp::to_string(emp::slugify(description_), "-key"))
  , getter(getter_)
  , renderer(renderer_)
  , divider(divider_)
  , cfg(cfg_)
  , last_update(std::numeric_limits<size_t>::max())
  {
    viewer_ << UI::Div(
      emp::slugify(emp::to_string(name, "card-holder"))
    ) << UI::Div().SetAttr(
        "class", "card text-center"
    ).SetAttr(
        "style", emp::to_string(
        "width: ",
        std::min(GetViewPortSize() - 100, 500) + 50,
        "px;"
      )
    ) << UI::Div(
      emp::slugify(emp::to_string(name, "card-header"))
    ).SetAttr(
      "class", "card-header"
    ) <<  name << UI::Close(
      emp::slugify(emp::to_string(name, "card-header"))
    ) << UI::Div().SetAttr(
      "class", "card-body"
    ) << canvas;
  }

  // for use as background in WebArtistConnection
  WebArtistCell(
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
    viewer->Div(
      emp::slugify(emp::to_string(name, "card-holder"))
    ).SetAttr("class", "collapse");
    description.SetCSS("display", "none");
  }

  void Activate() {
    const static auto boot_time {std::chrono::system_clock::now()};
    viewer->Div(
      emp::slugify(emp::to_string(name, "card-holder"))
    ).SetAttr(
      "class",
      ""
    ).SetCSS(
      "order",
      emp::to_string(
        std::chrono::duration_cast<std::chrono::milliseconds>(
           std::chrono::system_clock::now()  - boot_time
        ).count()
      )
    );
    description.SetCSS("display", "initial");
  }

  void Toggle() {
    if (description.GetCSS("display") == "none") Activate();
    else Deactivate();
  }

  std::string GetName() const { return name; }

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
      "black"
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
