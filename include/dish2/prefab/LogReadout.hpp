#pragma once
#ifndef DISH2_PREFAB_LOGREADOUT_HPP_INCLUDE
#define DISH2_PREFAB_LOGREADOUT_HPP_INCLUDE

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
