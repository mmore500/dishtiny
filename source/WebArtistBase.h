#pragma once

#include <optional>
#include <limits>

#include "web/Animate.h"
#include "web/Canvas.h"
#include "web/DocuExtras.h"
#include "web/color_map.h"

namespace UI = emp::web;

EM_JS(int, GetViewPortSize, (), {
 return Math.min(
   Math.max(document.documentElement.clientWidth, $(window).width(), window.innerWidth || 0),
   Math.max(document.documentElement.clientHeight, $(window).height(), window.innerHeight || 0)
  );
});

class WebArtistBase {
protected:
  const std::string name;

  UI::Canvas canvas;
  UI::Document* viewer;
  UI::DocuExtras description;

  const Config &cfg;

  size_t last_update;

public:

  virtual ~WebArtistBase() {};

  virtual void Deactivate();
  virtual void Activate();
  virtual void Toggle();
  virtual std::string GetName() const;

  virtual void Redraw(const size_t update) = 0;
  virtual void Download(const std::string & fn) = 0;


  
  WebArtistBase(
    std::string name_,
    std::string description_,
    UI::Document & viewer_,
    const Config &cfg_
  ) : name(name_),
      canvas(
        std::min(GetViewPortSize() - 100, 500),
        std::min(GetViewPortSize() - 100, 500)
      ),
      viewer(&viewer_),
      description(emp::to_string(emp::slugify(description_), "-key")),
      cfg(cfg_),
      last_update(std::numeric_limits<size_t>::max())
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
  
  // for use in WebArtistConnection
  WebArtistBase(
    std::string name_,
    std::string description_,
    UI::Canvas &canvas_,
    const Config &cfg_
  ) : name(name_),
      canvas(canvas_),
      description(emp::to_string(emp::slugify(description_), "-key")),
      cfg(cfg_),
      last_update(std::numeric_limits<size_t>::max())
    { ; }

};
void WebArtistBase::Deactivate() {
  viewer->Div(
    emp::slugify(emp::to_string(name, "card-holder"))
  ).SetAttr("class", "collapse");
  description.SetCSS("display", "none");
}
void WebArtistBase::Activate() {
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
void WebArtistBase::Toggle() {
  if (description.GetCSS("display") == "none") Activate();
  else Deactivate();
}
std::string WebArtistBase::GetName() const { return name; }