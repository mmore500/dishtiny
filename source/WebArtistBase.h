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

public:

  virtual ~WebArtistBase() {};
  virtual void Deactivate() = 0;
  virtual void Activate() = 0;
  virtual void Toggle() = 0;
  virtual std::string GetName() const = 0;
  virtual void Redraw(const size_t update) = 0;
  virtual void Download(const std::string & fn) = 0;

};
