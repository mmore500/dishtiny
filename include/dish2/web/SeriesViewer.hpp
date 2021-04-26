#pragma once
#ifndef DISH2_WEB_SERIESVIEWER_HPP_INCLUDE
#define DISH2_WEB_SERIESVIEWER_HPP_INCLUDE

#include <algorithm>
#include <chrono>
#include <ratio>
#include <string>
#include <tuple>

#include "../../../third-party/conduit/include/uitsl/countdown/runtime.hpp"
#include "../../../third-party/Empirical/include/emp/base/optional.hpp"
#include "../../../third-party/Empirical/include/emp/base/vector.hpp"
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
  typename Artist, typename Category,
  size_t SeriesLength, bool InitiallyActivated=false
>
class SeriesViewer {

  emp::vector< std::tuple<
    Artist, emp::web::Canvas, bool
  > > artists;

  bool is_active{ InitiallyActivated };
  // emp::web::DocuExtras description;

  emp::web::Canvas canvas{
  };

  std::string MakeID( const std::string& descriptor ) const {
    const std::string this_slug = emp::slugify( std::string{
      Artist::GetName()
    } );
    return emp::to_string( this_slug, "-", descriptor );
  }

  void SetupOne( const dish2::ThreadWorld<Spec>& thread_world ) {
    const size_t i = artists.size();

    artists.emplace_back(
      Artist{ thread_world, i },
      emp::web::Canvas{
        static_cast<double>( std::min(emp::GetViewPortSize() - 100, 500) ),
        static_cast<double>( std::min(emp::GetViewPortSize() - 100, 500) )
      },
      false
    );

    document_handles.at( "grid_viewer" )->Div(
      MakeID( "card-body" )
    ) << emp::web::Div(
      MakeID( emp::to_string("card-button", i) )
    ).SetAttr(
      "type", "button",
      "aria-pressed", "false",
      "autocomplete", "off"
    ).OnClick(
      [this, i](){ Toggle(i); }
    ) << i << emp::web::Close(
      MakeID( emp::to_string( "card-button", i) )
    ) << emp::web::Div(
      MakeID( emp::to_string( "canvas-holder", i) )
    ) << std::get< emp::web::Canvas >( artists.back() );

    Deactivate( i );
  }

public:

  SeriesViewer( const dish2::ThreadWorld<Spec>& thread_world )
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
      MakeID( "card-button" )
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
    ) << emp::web::Div(
      MakeID( "card-body" )
    ).SetAttr(
      "class", "card-body"
    );

    for (size_t i{}; i < SeriesLength; ++i) SetupOne( thread_world );

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

  void Activate( const size_t i ) {
    document_handles.at( "grid_viewer" )->Div(
      MakeID( emp::to_string( "canvas-holder", i ) )
    ).SetAttr(
      "class", ""
    );
    document_handles.at( "grid_viewer" )->Div(
      MakeID( emp::to_string("card-button", i) )
    ).SetAttr(
      "class", "btn btn-lg btn-block btn-primary active mb-2 mt-2"
    );
    auto& [artist, canvas, one_is_active] = artists[i];
    one_is_active = true;
    artist.Draw( canvas );
  }

  void Deactivate( const size_t i ) {
    document_handles.at( "grid_viewer" )->Div(
      MakeID( emp::to_string("card-button", i) )
    ).SetAttr(
      "class", "btn btn-lg btn-primary m-2"
    );
    document_handles.at( "grid_viewer" )->Div(
      MakeID( emp::to_string( "canvas-holder", i ) )
    ).SetAttr(
      "class", "collapse"
    );
    auto& [artist, canvas, one_is_active] = artists[i];
    one_is_active = false;
  }

  bool IsActivated() const { return is_active; }

  bool IsActivated( const size_t i ) const {
    const auto& [artist, canvas, one_is_active] = artists[i];
    return one_is_active;
  }

  void Toggle() {
    if ( IsActivated() ) Deactivate();
    else Activate();
  }

  void Toggle( const size_t i ) {
    if ( IsActivated( i ) ) Deactivate( i );
    else Activate( i );
  }

  void Redraw() {
    for ( auto& [artist, canvas, one_is_active] : artists ) {
      if (one_is_active) artist.Draw( canvas );
    }
  }

};

} // namespace dish2

#endif // #ifndef DISH2_WEB_SERIESVIEWER_HPP_INCLUDE
