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

  size_t update{};

  void SetupStepButton() {
    button_dash.Div("button_row") << emp::web::Div(
      "step_col"
    ).SetAttr(
      "class", "col-lg-auto p-2"
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

  void SetupRenderButton() {
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
    ) << emp::web::Button(
      [this](){ animator.ToggleRender(); },
      "Render every"
    ).SetAttr(
      "aria-pressed", "true"
    ).SetAttr(
      "class", "btn active btn-primary"
    ).SetAttr(
      "data-toggle", "button"
    ).SetAttr(
      "autocomplete", "off"
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

  }

  size_t GetRenderFreq() {
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
    std::function<size_t(const bool, const size_t)> update_and_render_callback
  ) : animator(
    [this, update_and_render_callback](const bool render_toggle){
      const size_t cur_update = update_and_render_callback(
        render_toggle, GetRenderFreq()
      );
      RefreshUpdateButton( cur_update );
    }
  ) {

    button_dash << emp::web::Div(
      "button_row"
    ).SetAttr(
      "class", "row justify-content-md-center"
    );

    SetupStepButton();
    SetupRunButton();
    SetupRenderButton();

    RefreshUpdateButton( 0 );

  }

};

} // namespace dish2



#endif // #ifndef DISH2_WEB_CONTROLPANEL_HPP_INCLUDE
