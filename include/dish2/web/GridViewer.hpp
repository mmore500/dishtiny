#pragma once
#ifndef DISH2_WEB_GRIDVIEWER_HPP_INCLUDE
#define DISH2_WEB_GRIDVIEWER_HPP_INCLUDE

#include <algorithm>

#include "../../../third-party/Empirical/source/base/optional.h"
#include "../../../third-party/Empirical/source/web/Canvas.h"
#include "../../../third-party/Empirical/source/web/commands.h"
#include "../../../third-party/Empirical/source/web/Div.h"
#include "../../../third-party/Empirical/source/web/js_utils.h"

#include "../world/ThreadWorld.hpp"
#include "../spec/Spec.hpp"

namespace dish2 {

template<typename Artist>
class GridViewer {

  Artist artist;

  emp::web::Canvas canvas{
    static_cast<double>( std::min(emp::GetViewPortSize() - 100, 500) ),
    static_cast<double>( std::min(emp::GetViewPortSize() - 100, 500) )
  };

public:

  GridViewer(
    const dish2::ThreadWorld<dish2::Spec>& thread_world,
    emp::web::Document& parent
  ) : artist( thread_world )
  {

    parent << emp::web::Div(
      emp::slugify(emp::to_string(Artist::GetName(), "card-holder"))
    ).SetCSS(
      "padding-bottom",
      "1.5em"
    ) << emp::web::Div().SetAttr(
        "class", "card text-center"
    ).SetAttr(
        "style", emp::to_string(
        "width: ",
        std::min(emp::GetViewPortSize() - 100, 500) + 50,
        "px;"
      )
    ) << emp::web::Div(
      emp::slugify(emp::to_string(Artist::GetName(), "card-header"))
    ).SetAttr(
      "class", "card-header"
    ) <<  Artist::GetName() << emp::web::Close(
      emp::slugify(emp::to_string(Artist::GetName(), "card-header"))
    ) << emp::web::Div().SetAttr(
      "class", "card-body"
    ) << canvas;

  }

  void Redraw() { artist.Draw( canvas ); }

};

} // namespace dish2

#endif // #ifndef DISH2_WEB_GRIDVIEWER_HPP_INCLUDE
