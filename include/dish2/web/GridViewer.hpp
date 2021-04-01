#pragma once
#ifndef DISH2_WEB_GRIDVIEWER_HPP_INCLUDE
#define DISH2_WEB_GRIDVIEWER_HPP_INCLUDE

#include <algorithm>
#include <chrono>
#include <ratio>
#include <string>

#include "../../../third-party/conduit/include/uitsl/countdown/runtime.hpp"
#include "../../../third-party/Empirical/include/emp/base/optional.hpp"
#include "../../../third-party/Empirical/include/emp/tools/string_utils.hpp"
#include "../../../third-party/Empirical/include/emp/web/Canvas.hpp"
#include "../../../third-party/Empirical/include/emp/web/commands.hpp"
#include "../../../third-party/Empirical/include/emp/web/Div.hpp"
#include "../../../third-party/Empirical/include/emp/web/DocuExtras.hpp"
#include "../../../third-party/Empirical/include/emp/web/Element.hpp"
#include "../../../third-party/Empirical/include/emp/web/js_utils.hpp"

#include "../world/ThreadWorld.hpp"

#include "DocumentHandles.hpp"

namespace dish2 {

template<
  typename Spec,
  typename Artist, typename Category, bool InitiallyActivated=false
>
class GridViewer {

  Artist artist;

  bool is_active{ InitiallyActivated };
  // emp::web::DocuExtras description;

  emp::web::Canvas canvas{
    static_cast<double>( std::min(emp::GetViewPortSize() - 100, 500) ),
    static_cast<double>( std::min(emp::GetViewPortSize() - 100, 500) )
  };

  std::string MakeID( const std::string& descriptor ) const {
    const std::string this_slug = emp::slugify(
      std::string{ Artist::GetName() }
    );
    return emp::to_string( this_slug, "-", descriptor );
  }

public:

  GridViewer( const dish2::ThreadWorld<Spec>& thread_world )
  : artist( thread_world )
  // , description( MakeID("key") )
  {

    *document_handles.at( "grid_viewer" ) << emp::web::Div(
      MakeID( "card-holder" )
    ).SetCSS(
      "padding-bottom", "1.5em"
    ) << emp::web::Div(
      MakeID( "card" )
    ).SetAttr(
      "class", "card text-center"
    ).SetAttr(
      "style", emp::to_string(
        "width: ", std::min(emp::GetViewPortSize() - 100, 500) + 50, "px;"
      )
    ) << emp::web::Div(
      MakeID( "card-header" )
    ).SetAttr(
      "class", "card-header"
    ) << emp::web::Div(
      emp::slugify(emp::to_string(Artist::GetName(), "card-button"))
    ).SetAttr(
      "class", "btn btn-lg btn-block btn-primary text-left active",
      "type", "button",
      "aria-pressed", "true",
      "autocomplete", "off"
    ).OnClick(
      [this](){ Deactivate(); }
    ) << Artist::GetName();

    document_handles.at( "grid_viewer" )->Div(
      MakeID( "card" )
    ) << emp::web::Div().SetAttr(
      "class", "card-body"
    ) << canvas;

    *document_handles.at( Category::GetID() ) << emp::web::Div(
      MakeID( "selector" )
    ).SetAttr(
      "type", "button",
      "data-toggle", "",
      "autocomplete", "off"
    ).OnClick(
      [this](){ Toggle(); Redraw(); }
    ) << Artist::GetName();

    if ( is_active ) Activate();
    else Deactivate();

  }

  void Deactivate() {

    is_active = false;

    document_handles.at( "grid_viewer" )->Div(
      MakeID( "card-holder" )
    ).SetAttr(
      "class", "collapse"
    );
    // description.SetCSS("display", "none");
    document_handles.at( Category::GetID() )->Div(
      MakeID( "selector" )
    ).SetAttr(
      "class", "btn-lg btn-block btn-primary text-left",
      "aria-pressed", "false"
    );
  }

  void Activate() {

    is_active = true;

    document_handles.at( "grid_viewer" )->Div(
      MakeID( "card-holder" )
    ).SetAttr(
      "class", ""
    ).SetCSS(
      "order",
      emp::to_string( uitsl::runtime< std::chrono::duration<
        double, std::milli
      > >.GetElapsed().count() )
    );
    // description.SetCSS("display", "initial");
    document_handles.at( Category::GetID() )->Div(
      MakeID( "selector" )
    ).SetAttr(
      "class", "btn-lg btn-block btn-primary text-left active",
      "aria-pressed", "false"
    );
    Redraw();
  }

  bool IsActivated() const { return is_active; }

  void Toggle() {
    if ( IsActivated() ) Deactivate();
    else Activate();
  }

  void Redraw() { artist.Draw( canvas ); }

};

} // namespace dish2

#endif // #ifndef DISH2_WEB_GRIDVIEWER_HPP_INCLUDE
