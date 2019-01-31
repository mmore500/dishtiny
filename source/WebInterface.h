#pragma once

#include <experimental/optional>
#include <string>

#include "tools/math.h"

#include "Config.h"
#include "DishWorld.h"
#include "WebArtist.h"

namespace UI = emp::web;

class WebInterface : public UI::Animate {

  Config cfg;

  UI::Document viewer;        //< Div that contains the canvas viewer.
  UI::Document button_dash;  //< Div that contains the button dashboard.
  UI::Canvas canvas;

  DishWorld w;

  WebArtist<ChannelPack> channel;


public:

  WebInterface()
    : viewer("emp_viewer")
    , button_dash("emp_button_dash")
    , canvas(500,500)
    , w(cfg)
    , channel(
        canvas,
        [this](size_t i){
          return w.IsOccupied(i) ? std::experimental::make_optional(w.man->Channel(i).GetIDs()) : std::experimental::nullopt;
        },
        [](std::experimental::optional<ChannelPack> cp) {
          return cp ? emp::ColorHSL((*cp)[0]%360,emp::Mod((*cp)[1],50.0),60.0): emp::ColorRGB(0.0,0.0,0.0);
        },
        cfg
    ) {

    viewer << canvas;
    viewer << "<br />";

    auto ud_text = viewer.AddText("ud_text");
    ud_text << "Update: " << UI::Live(w.update);

    viewer << UI::Button(
      [this](){
        std::cout << "pre button" << std::endl;
        w.Update();
        std::cout << "post button" << std::endl;
        Redraw();
      },
      "update"
    );

  }

  void Redraw() {
    std::cout << "Web Interface redraw" << std::endl;
    viewer.Text("ud_text").Redraw();
    channel.Redraw();
  }

};
