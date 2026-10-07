#pragma once
#ifndef DISH2_WEB_CONTROLPANEL_HPP_INCLUDE
#define DISH2_WEB_CONTROLPANEL_HPP_INCLUDE

#include <functional>

#include "../../../third-party/conduit/include/uitsl/math/math_utils.hpp"
#include "../../../third-party/Empirical/include/emp/web/Button.hpp"
#include "../../../third-party/Empirical/include/emp/web/Div.hpp"
#include "../../../third-party/Empirical/include/emp/web/Document.hpp"
#include "../../../third-party/Empirical/include/emp/web/emfunctions.hpp"

#include "Animator.hpp"

namespace dish2 {

class ControlPanel {

  emp::web::Document button_dash{ "emp_button_dash" };

  dish2::Animator animator;

  std::function<size_t()> update_callback;
  std::function<void()> render_callback;
  std::function<void()> download_callback;

  // render and download can only be selected while every is selected
  bool every_toggle{ true };
  bool render_toggle{ true };
  bool download_toggle{ false };

  size_t update{};

  void SetupStepButton() {
    button_dash.Div("button_row") << emp::web::Div(
      "step_col"
    ).SetAttr(
      "class", "col-lg-auto p-2"
    ).SetCSS(
      "margin-right", "0.5rem"
    ) << emp::web::Div(
      "step_button"
    ).SetAttr(
      "class", "btn btn-block btn-lg btn-primary"
    ).OnClick(
      [this](){ animator.DoFrame(); }
    ) << "Update&nbsp;"
    << emp::web::Element(
      "span"
    ).SetCSS(
      // adapted from http://code.iamkate.com/html-and-css/fixing-browsers-broken-monospace-font-handling/
      "font-family", "monospace,monospace",
      "font-size", "1em"
    ) << emp::web::Text(
      "update_text"
    );

  }

  void SetupRunButton() {

    button_dash.Div("button_row") << emp::web::Div(
      "run_col"
    ).SetAttr(
      "class", "col-lg-auto p-2"
    ).SetCSS(
      "margin-right", "0.5rem"
    ) << emp::web::Button(
      [this](){
        animator.ToggleActive();

        auto button = button_dash.Button( "run-button" );
        button.Freeze();
        if ( animator.GetActive() ) {
          button.SetCSS(
              "min-width",
              emp::to_string( button.GetWidth(), "px" )
          ).SetAttr(
            "class", "btn btn-primary btn-block btn-lg active",
            "aria-pressed", "true"
          ).SetLabel( "Stop" );
        } else if (
          !animator.GetActive()
        ) {
          button.SetCSS(
            "min-width",
            emp::to_string( button.GetWidth(), "px" )
          ).SetAttr(
            "class", "btn btn-primary btn-block btn-lg",
            "aria-pressed", "false"
          ).SetLabel( "Start" );
        }
        button.Activate();
      },
      "Start",
      "run-button"
    ).SetAttr(
      "class", "btn btn-primary btn-block btn-lg",
      "aria-pressed", "false"
    );

  }

  void SetButtonActive( const std::string& id, const bool active ) {
    button_dash.Button( id ).SetAttr(
      "class", active ? "btn btn-primary active" : "btn btn-primary",
      "aria-pressed", active ? "true" : "false"
    );
  }

  void RefreshToggleButtons() {
    SetButtonActive( "render-button", render_toggle );
    SetButtonActive( "download-button", download_toggle );
    SetButtonActive( "every-button", every_toggle );
  }

  // if every is selected, render and download toggle; otherwise, they act once
  void ClickRender() {
    if ( every_toggle ) render_toggle = !render_toggle;
    else render_callback();
    RefreshToggleButtons();
  }

  void ClickDownload() {
    if ( every_toggle ) download_toggle = !download_toggle;
    else download_callback();
    RefreshToggleButtons();
  }

  // unselecting every unselects render and download, selecting every does not
  // reselect them
  void ClickEvery() {
    every_toggle = !every_toggle;
    if ( !every_toggle ) render_toggle = download_toggle = false;
    RefreshToggleButtons();
  }

  void SetupRenderDownloadEveryButtons() {
    button_dash.Div("button_row") << emp::web::Div(
      "render_col"
    ).SetAttr(
      "class", "col-lg-auto p-2"
    ) << emp::web::Div(
      "render-wrapper"
    ).SetAttr(
      "class", "input-group input-group-lg btn-block"
    ) << emp::web::Div(
      "render_input-prepend"
    ).SetAttr(
      "class", "input-group-prepend"
    );

    // no box-shadow, so a clicked button doesn't keep a focus box around it
    button_dash.Div("render_input-prepend") << emp::web::Button(
      [this](){ ClickRender(); }, "Render", "render-button"
    ).SetCSS(
      "box-shadow", "none"
    );
    // small gaps between the buttons, which stay square-edged
    button_dash.Div("render_input-prepend") << emp::web::Button(
      [this](){ ClickDownload(); }, "Download", "download-button"
    ).SetCSS(
      "margin-left", "2px",
      "border-radius", "0",
      "box-shadow", "none"
    );
    button_dash.Div("render_input-prepend") << emp::web::Button(
      [this](){ ClickEvery(); }, "Every", "every-button"
    ).SetCSS(
      "margin-left", "2px",
      "border-radius", "0",
      "box-shadow", "none"
    );

    button_dash.Div("render-wrapper") << emp::web::Input(
      [](std::string){ ; },
      "number",
      "",
      "render_frequency"
    ).Checker(
      [](const std::string in){
        return emp::is_digits(in) && in.size() && std::stoi(in) > 0;
      }
    ).Value(
      "16"
    ).Min(
      "1"
    ).Max(
      emp::to_string(std::numeric_limits<size_t>::max())
    ).Step(
      "1"
    ).SetAttr(
      "class", "form-control"
    ).SetCSS(
      "min-width", "96px"
    );

    button_dash.Div("render-wrapper") << emp::web::Div(
      "input-group-append"
    ).SetAttr(
      "class", "input-group-append"
    ) << emp::web::Text().SetAttr(
      "class", "input-group-text"
    ) << "th update";

    RefreshToggleButtons();

  }

  size_t GetEveryFreq() {
    return uitsl::stoszt(
      button_dash.Input("render_frequency").GetCurrValue()
    );
  }

  void RefreshUpdateButton(const size_t update) {

    auto text = button_dash.Text( "update_text" );
    text.Clear();
    text << emp::to_string(update);
  }

public:

  ControlPanel(
    std::function<size_t()> update_callback_,
    std::function<void()> render_callback_,
    std::function<void()> download_callback_
  ) : animator(
    [this](){
      const size_t cur_update = update_callback();
      if ( cur_update % GetEveryFreq() == 0 ) {
        if ( render_toggle ) render_callback();
        if ( download_toggle ) download_callback();
      }
      RefreshUpdateButton( cur_update );
    }
  ), update_callback( update_callback_ )
  , render_callback( render_callback_ )
  , download_callback( download_callback_ )
  {

    button_dash << emp::web::Div(
      "button_row"
    ).SetAttr(
      "class", "row justify-content-md-center"
    );

    SetupStepButton();
    SetupRunButton();
    SetupRenderDownloadEveryButtons();

    RefreshUpdateButton( 0 );

  }

};

} // namespace dish2



#endif // #ifndef DISH2_WEB_CONTROLPANEL_HPP_INCLUDE
