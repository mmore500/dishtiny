#pragma once
#ifndef DISH2_WEB_EXECUTIONLOGPANEL_HPP_INCLUDE
#define DISH2_WEB_EXECUTIONLOGPANEL_HPP_INCLUDE

#include "../../../third-party/Empirical/source/web/Document.h"

#include "../prefab/LogEventReadout.hpp"
#include "../prefab/LogStackReadout.hpp"

namespace dish2 {

class ExecutionLogPanel {

  emp::web::Document panel{ "execution-log-panel" };

  dish2::LogEventReadout log_events_readout;

  dish2::LogStackReadout log_stack_readout;

public:

  ExecutionLogPanel() {
    panel.SetAttr(
      "class", "list-group list-group-flush collapse"
    );

    panel <<  emp::web::Div().SetAttr(
      "class",
      "list-group-item list-group-item-info"
    ) << "‼️ When this panel is open, you'll have to click the step button to advance the simulation.";

    panel << emp::web::Div().SetAttr(
      "class",
      "list-group-item list-group-item-action list-group-item-success"
    ).SetCSS(
      "cursor", "pointer"
    ).OnClick(
      [](){ }
    ) << "Step";



    panel <<  emp::web::Div().SetAttr(
      "class",
      "list-group-item"
    ) << "Stack Log --- tells you the nested set of tasks that the computer is working on right now.";

    panel <<  emp::web::Div().SetAttr(
      "class",
      "list-group-item"
    ) << (emp::web::Div) log_stack_readout;

    panel <<  emp::web::Div().SetAttr(
      "class",
      "list-group-item"
    ) << "Events Log --- gives you a play-by-play of what the computer has done.";

    panel <<  emp::web::Div().SetAttr(
      "class",
      "list-group-item"
    ).SetCSS(
      "max-height: 80vh",
      "display: flex"
    ) << (emp::web::Div) log_events_readout;

  }

};

} // namespace dish2

#endif // #ifndef DISH2_WEB_EXECUTIONLOGPANEL_HPP_INCLUDE
