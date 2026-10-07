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
 * Expandable pill showing a name and a live snippet of text.
 *
 * When expanded, shows the description and a bar containing the snippet
 * alongside a button to copy it to the clipboard.
 */
struct SnippetPill : public emp::web::Div {

  SnippetPill(
    const std::string & title,
    const std::function<std::string()> snippet,
    const std::string & description,
    const bool split=true
  ) {

    const std::string slug = emp::slugify( title );
    const std::string code_id = emp::to_string( "snippetpill-code-", slug );

    this->SetAttr(
      "class", split ? "col-md-6 p-3" : "col-md-6 col-lg-4 col-xl-3 p-3"
    );

    *this << emp::web::Div(
      ).SetAttr(
        "class", "card"
      ) << emp::web::Div(
        emp::to_string("snippetpill-header-", slug)
      ).SetAttr(
        "class", "card-header"
      ) << emp::web::Div(
        emp::to_string("snippetpill-wrapper-", slug)
      ).SetAttr(
        "data-toggle", "collapse",
        "href", emp::to_string("#snippetpill-collapse-", slug)
      ) << emp::web::Div(
        emp::to_string("snippetpill-wrapper2-", slug)
      ) << emp::web::Element(
        "button", emp::to_string("snippetpill-button-", slug)
      ).SetAttr(
        "class", "btn btn-block btn-primary p-0 border-0",
        "data-toggle", "button"
      ) << emp::web::Div(
        emp::to_string("snippetpill-btngroup-", slug)
      ).SetAttr(
        "class", "btn-group w-100",
        "role", "group"
      ) << emp::web::Div(
        emp::to_string("snippetpill-active-", slug)
      ).SetAttr(
        "class", "btn w-100 btn-primary border-secondary"
      ).SetCSS(
        "max-width", "75%"
      ) << title << emp::web::Close(
        emp::to_string("snippetpill-active-", slug)
      ) << emp::web::Div(
        emp::to_string("snippetpill-value-", slug)
      ).SetAttr(
        "class", "badge-light btn w-25 border-secondary"
      ) << emp::web::Live(
        snippet
      ) << emp::web::Close(
        emp::to_string("snippetpill-value-", slug)
      ) << emp::web::Close(
        emp::to_string("snippetpill-btngroup-", slug)
      ) << emp::web::Close(
        emp::to_string("snippetpill-button-", slug)
      ) << emp::web::Close(
        emp::to_string("snippetpill-wrapper2-", slug)
      ) << emp::web::Close(
        emp::to_string("snippetpill-wrapper-", slug)
      ) << emp::web::Close(
        emp::to_string("snippetpill-header-", slug)
      ) << emp::web::Div(
        emp::to_string("snippetpill-collapse-", slug)
      ).SetAttr(
        "class", "card-body collapse"
      ) << description
        << emp::web::Div(
          emp::to_string("snippetpill-copybar-", slug)
        ).SetAttr(
          "class", "input-group mt-3 w-100"
        ) << emp::web::Div(
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
