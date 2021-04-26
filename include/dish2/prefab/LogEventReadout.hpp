#pragma once
#ifndef DISH2_PREFAB_LOGEVENTREADOUT_HPP_INCLUDE
#define DISH2_PREFAB_LOGEVENTREADOUT_HPP_INCLUDE

#include <algorithm>
#include <numeric>
#include <ratio>
#include <string>
#include <utility>

#include "../../../third-party/conduit/include/uitsl/algorithm/for_each.hpp"
#include "../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../third-party/Empirical/include/emp/tools/string_utils.hpp"
#include "../../../third-party/Empirical/include/emp/web/commands.hpp"
#include "../../../third-party/Empirical/include/emp/web/Div.hpp"
#include "../../../third-party/Empirical/include/emp/web/Document.hpp"
#include "../../../third-party/Empirical/include/emp/web/Element.hpp"
#include "../../../third-party/Empirical/include/emp/web/TextFeed.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/utility/CountingIterator.hpp"

#include "../debug/log_event_dispatcher.hpp"
#include "../debug/log_stack.hpp"

namespace dish2 {

class LogEventReadout {

  emp::web::Div container;

  emp::web::TextFeed log{
    "",
    "<hr>",
    std::kilo::num
  };

  void setup() {

    auto append_context = container.SetAttr(
      "class", "p-3 mlr-3"
    ).SetCSS(
      "flex-grow", "1",
      "position", "relative",
      "overflow", "hidden",
      "background", "#4C566A",
      "-webkit-mask-image", "-webkit-gradient(linear,left 25%,left top,from(#4C566A),to(transparent))",
      "mask-image", "-webkit-gradient(linear,left 25%,left top,from(#4C566A),to(transparent))",
      "font-size", "16px",
      "color", "#ECEFF4",
      "font-family", "monospace",
      "margin", "30px auto 0",
      "padding", "20px",
      "box-sizing", "border-box",
      "border-radius", "4px",
      "display", "flex",
      "flex-wrap", "wrap-reverse",
      "flex-direction", "column-reverse",
      "width", "100%"
    ) << emp::web::Div().SetCSS(
      "margin-top", "40px",
      "margin-bottom", "0px"
    );

    append_context << log;

    append_context << emp::web::Div() << emp::web::Element(
      "span"
    ).SetAttr(
      "class", "spinner-grow spinner-grow-sm align-middle",
      "role", "status",
      "aria-hidden", "true"
    );

  }

  void register_log_callback() {
    dish2::log_event_dispatcher.push_back(
      [this](const dish2::LogEntry& entry, const size_t stack_depth) {

        log << emp::to_string(
          emp::repeat("📌", stack_depth),
          " ",
          entry.type.symbol,
          " ",
          entry.type.name,
          " ",
          entry.name,
          "<a class=\"float-right\" href=\"javascript:;\" data-placement=\"left\" data-toggle=\"popover\" data-trigger=\"manual\" title=\"",
          entry.name,
          "\" data-content=\"",
          entry.detail,
          "\" onclick=\"$(this).popover(); $(this).popover('toggle');\">🤔</a>"
        );
        // TODO entry.detail tooltip?

      }
    );
  }

public:

  LogEventReadout() {
    setup();
    register_log_callback();
  }

  operator emp::web::Div&() { return container; }

  void Clear() { log.Clear(); }

  void Redraw() { log.Redraw(); }

};


} // namespace dish2

#endif // #ifndef DISH2_PREFAB_LOGEVENTREADOUT_HPP_INCLUDE
