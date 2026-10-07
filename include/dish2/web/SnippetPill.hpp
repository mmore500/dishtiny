#pragma once
#ifndef DISH2_WEB_SNIPPETPILL_HPP_INCLUDE
#define DISH2_WEB_SNIPPETPILL_HPP_INCLUDE

#include <functional>
#include <string>

#include "../../../third-party/Empirical/include/emp/tools/string_utils.hpp"
#include "../../../third-party/Empirical/include/emp/web/commands.hpp"
#include "../../../third-party/Empirical/include/emp/web/Div.hpp"
#include "../../../third-party/Empirical/include/emp/web/Element.hpp"
#include "../../../third-party/Empirical/include/emp/web/init.hpp"

namespace dish2 {

/**
 * Bar showing a live snippet of text alongside a button to copy it to the
 * clipboard.
 *
 * Meant to be added to the dropdown body of a DataPill.
 */
struct SnippetPill : public emp::web::Div {

  SnippetPill(
    const std::string & title,
    const std::function<std::string()> snippet
  ) : emp::web::Div(
    emp::to_string("snippetpill-copybar-", emp::slugify( title ))
  ) {

    const std::string slug = emp::slugify( title );
    const std::string code_id = emp::to_string( "snippetpill-code-", slug );

    this->SetAttr(
      "class", "input-group mt-3 w-100"
    );

    *this << emp::web::Div(
      code_id
    ).SetAttr(
      "class", "form-control"
    ).SetCSS(
      "font-family", "monospace",
      "height", "auto",
      "overflow-x", "auto",
      "text-align", "center",
      "white-space", "nowrap"
    ) << emp::web::Live(
      snippet
    ) << emp::web::Close(
      code_id
    ) << emp::web::Div(
      emp::to_string("snippetpill-copyappend-", slug)
    ).SetAttr(
      "class", "input-group-append"
    ) << emp::web::Element(
      "button", emp::to_string("snippetpill-copy-", slug)
    ).SetAttr(
      "class", "btn btn-primary",
      "type", "button",
      // confirm with a bootstrap tooltip that disappears after a moment
      "onclick", emp::to_string(
        "var b = this; var show = function(msg) { ",
        "$(b).tooltip('dispose'); ",
        "$(b).tooltip({title: msg, trigger: 'manual', placement: 'top'});",
        "$(b).tooltip('show'); ",
        "setTimeout(function() { $(b).tooltip('dispose'); }, 1500); }; ",
        "navigator.clipboard.writeText(document.getElementById('",
        code_id, "').textContent.trim()).then(",
        "function() { show('Copied!'); }, ",
        "function() { show('Copy failed'); });"
      )
    ) << "<i class=\"bi bi-copy\"></i>&nbsp;Copy";

  }

};

} // namespace dish2

#endif // #ifndef DISH2_WEB_SNIPPETPILL_HPP_INCLUDE
