#pragma once
#ifndef DISH2_PREFAB_LOGREADOUT_HPP_INCLUDE
#define DISH2_PREFAB_LOGREADOUT_HPP_INCLUDE

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
#include "../../../third-party/Empirical/include/emp/web/Text.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/utility/CountingIterator.hpp"

#include "LogEventReadout.hpp"
#include "LogStackReadout.hpp"

namespace dish2 {

class LogReadout {

  emp::web::Div container;

  dish2::LogEventReadout log_events_readout;

  dish2::LogStackReadout log_stack_readout;

  void setup() {

    container.SetCSS(
      "height", "60vh",
      "display", "flex",
      "flex-flow", "column"
    );

    container << (emp::web::Div) log_stack_readout;

    container << (emp::web::Div) log_events_readout;

  }


public:

  LogReadout() { setup(); }

  operator emp::web::Div&() { return container; }

};


} // namespace dish2

#endif // #ifndef DISH2_PREFAB_LOGREADOUT_HPP_INCLUDE
