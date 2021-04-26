#pragma once
#ifndef DISH2_PREFAB_MODALLOGREADOUT_HPP_INCLUDE
#define DISH2_PREFAB_MODALLOGREADOUT_HPP_INCLUDE

#include "../../../third-party/Empirical/include/emp/web/Div.hpp"
#include "../../../third-party/Empirical/include/emp/web/Document.hpp"
#include "../../../third-party/Empirical/include/emp/web/Element.hpp"

#include "LogReadout.hpp"

namespace dish2 {

class ModalLogReadout {

  emp::web::Div modal;

  dish2::LogReadout readout;

public:

  ModalLogReadout( const std::string& id="modal-log-readout" )
  : modal( id ) {

    auto append_context =
    modal.SetAttr(
      "class", "modal fade bd-example-modal-lg",
      "tabindex", "-1",
      "role", "dialog",
      "aria-labelledby", "progress console",
      "aria-hidden", "true",
      "data-backdrop", "static", // prevent modal close via background click
      "data-keyboard", "false" // prevent modal close via keyboard press
    ) << emp::web::Div().SetAttr(
      "class", "modal-dialog modal-dialog-centered modal-lg"
    ) << emp::web::Div().SetAttr(
      "class", "modal-content"
    );

    append_context << emp::web::Div().SetAttr(
      "class", "modal-header"
    ) << emp::web::Div().SetAttr(
      "class", "btn btn-primary btn-lg btn-block",
      "type", "button",
      "disabled", "true"
    ) << "Working...";

    append_context << emp::web::Div().SetAttr(
      "class", "modal-body"
    ) << (emp::web::Div) readout;

    append_context << emp::web::Div().SetAttr(
      "class", "modal-footer alert-info d-block text-center"
    ) << emp::web::Font().SetItalic(
    ) << "Hang tight! This dialog will close automatically upon completion.";

  }

  operator emp::web::Div&() { return modal; }

};

} // namespace dish2

#endif // #ifndef DISH2_PREFAB_MODALLOGREADOUT_HPP_INCLUDE
