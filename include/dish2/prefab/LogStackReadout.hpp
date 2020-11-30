#pragma once
#ifndef DISH2_PREFAB_LOGSTACKREADOUT_HPP_INCLUDE
#define DISH2_PREFAB_LOGSTACKREADOUT_HPP_INCLUDE

#include <algorithm>
#include <numeric>
#include <ratio>
#include <string>
#include <utility>

#include "../../../third-party/conduit/include/uitsl/algorithm/for_each.hpp"
#include "../../../third-party/Empirical/source/base/vector.h"
#include "../../../third-party/Empirical/source/tools/string_utils.h"
#include "../../../third-party/Empirical/source/web/commands.h"
#include "../../../third-party/Empirical/source/web/Div.h"
#include "../../../third-party/Empirical/source/web/Document.h"
#include "../../../third-party/Empirical/source/web/Element.h"
#include "../../../third-party/Empirical/source/web/Text.h"
#include "../../../third-party/signalgp-lite/include/sgpl/utility/CountingIterator.hpp"

#include "../debug/log_stack_dispatcher.hpp"
#include "../debug/log_stack.hpp"

namespace dish2 {

class LogStackReadout {

  emp::web::Div container;
  emp::web::Div ratchet;

  emp::web::Text stack;

  emp::vector<double> ratchet_heights{ 0.0 };

  void setup() {

    container.SetAttr(
      "class", "p-3 mlr-3"
    ).SetCSS(
      "flex-grow", "0",
      "background", "#4C566A",
      "font-size", "16px",
      "color", "#ECEFF4",
      "font-family", "monospace",
      "padding", "20px",
      "box-sizing", "border-box",
      "border-radius", "4px",
      "width", "100%"
    ) << ratchet << stack;

  }

  void register_log_callback() {
    dish2::log_stack_dispatcher.push_back(
      [this](const dish2::log_stack_t& cur_stack) {

        if ( stack.GetSize() > cur_stack.size() ) {
          ratchet.SetCSS(
            "min-height", emp::to_string(ratchet_heights.back(), "px"),
            "transition", "min-height 3s ease-out"
          );
          stack.PopBack();
          stack.Redraw();
          ratchet_heights.pop_back();
        }

        else if ( stack.GetSize() < cur_stack.size() ) {
          stack << emp::to_string(
            stack.GetSize() ? "<hr>" : "",
            emp::repeat("📌", stack.GetSize()),
            "📎 ",
            cur_stack.back().name
          );
          ratchet_heights.push_back( ratchet.GetHeight() );
          std::cout << ratchet_heights.back() << std::endl;
        }

        else emp_assert( false );

      }
    );
  }

public:

  LogStackReadout() {
    setup();
    register_log_callback();
  }

  operator emp::web::Div&() { return container; }

};


} // namespace dish2

#endif // #ifndef DISH2_PREFAB_LOGSTACKREADOUT_HPP_INCLUDE
