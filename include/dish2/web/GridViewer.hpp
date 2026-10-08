#pragma once
#ifndef DISH2_WEB_GRIDVIEWER_HPP_INCLUDE
#define DISH2_WEB_GRIDVIEWER_HPP_INCLUDE

#include <algorithm>
#include <chrono>
#include <ostream>
#include <ratio>
#include <string>

#include "../../../third-party/conduit/include/uitsl/countdown/runtime.hpp"
#include "../../../third-party/Empirical/include/emp/base/optional.hpp"
#include "../../../third-party/Empirical/include/emp/tools/keyname_utils.hpp"
#include "../../../third-party/Empirical/include/emp/tools/string_utils.hpp"
#include "../../../third-party/Empirical/include/emp/web/Canvas.hpp"
#include "../../../third-party/Empirical/include/emp/web/commands.hpp"
#include "../../../third-party/Empirical/include/emp/web/Div.hpp"
#include "../../../third-party/Empirical/include/emp/web/DocuExtras.hpp"
#include "../../../third-party/Empirical/include/emp/web/Element.hpp"
#include "../../../third-party/Empirical/include/emp/web/js_utils.hpp"

#include "../world/ThreadWorld.hpp"

#include "DocumentHandles.hpp"
#include "saved_folder.hpp"
#include "toggle_view_key.hpp"

namespace dish2 {

template<
  typename Spec,
  typename Artist, typename Category, bool InitiallyActivated=false
>
class GridViewer {

  Artist artist;

  bool is_active{ InitiallyActivated };
  bool key_is_shown{ true };

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

  explicit GridViewer( const dish2::ThreadWorld<Spec>& thread_world )
  : artist( thread_world )
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
    key_is_shown = false;

    document_handles.at( "grid_viewer" )->Div(
      MakeID( "card-holder" )
    ).SetAttr(
      "class", "collapse"
    );
    dish2::toggle_view_key(
      emp::slugify( std::string{ Artist::GetName() } ), false
    );
    document_handles.at( Category::GetID() )->Div(
      MakeID( "selector" )
    ).SetAttr(
      "class", "btn-lg btn-block btn-primary text-left",
      "aria-pressed", "false"
    );
  }

  void Activate() {

    is_active = true;
    key_is_shown = true;

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
    dish2::toggle_view_key(
      emp::slugify( std::string{ Artist::GetName() } ), true
    );
    document_handles.at( Category::GetID() )->Div(
      MakeID( "selector" )
    ).SetAttr(
      "class", "btn-lg btn-block btn-primary text-left active",
      "aria-pressed", "false"
    );
    Redraw();
  }

  bool IsActivated() const { return is_active; }

  // selector clicks cycle: open view with key -> hide key -> close view
  void Toggle() {
    if ( !IsActivated() ) Activate();
    else if (
      key_is_shown && dish2::toggle_view_key(
        emp::slugify( std::string{ Artist::GetName() } ), false
      )
    ) key_is_shown = false;
    else Deactivate();
  }

  void Redraw() { artist.Draw( canvas ); }

  // saves a png and appends the underlying data to the saved folder
  void Save( const size_t update ) {
    emp::keyname::unpack_t attrs{
      {"title", emp::slugify( std::string{ Artist::GetName() } )},
      {"update", emp::to_string( update )},
      {"ext", ".png"}
    };
    dish2::saved_folder::save_png( canvas, emp::keyname::pack( attrs ) );
    dish2::saved_folder::save_data(
      [this, update]( std::ostream& out ){
        artist.Tabulate( out, emp::to_string(
          "{\"update\": ", update, ", \"artist\": \"", Artist::GetName(), "\"}"
        ) );
      }
    );
  }

};

} // namespace dish2

#endif // #ifndef DISH2_WEB_GRIDVIEWER_HPP_INCLUDE
