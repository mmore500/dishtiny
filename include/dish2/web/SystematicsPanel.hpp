#pragma once
#ifndef DISH2_WEB_SYSTEMATICSPANEL_HPP_INCLUDE
#define DISH2_WEB_SYSTEMATICSPANEL_HPP_INCLUDE

#include <functional>

#include "../../../third-party/conduit/include/uitsl/math/math_utils.hpp"
#include "../../../third-party/Empirical/source/web/Div.h"
#include "../../../third-party/Empirical/source/web/Document.h"

#include "DataPill.hpp"

namespace dish2 {

class SystematicsPanel {

  emp::web::Document button_dash{ "emp_systematics_dash" };

  void SetupDummyPill() {

    button_dash << (emp::web::Div) dish2::DataPill(
      "example title",
      [](){ return "42"; },
      "example description"
    );

  }

public:

  SystematicsPanel() {

    SetupDummyPill();

  }

};

} // namespace dish2



#endif // #ifndef DISH2_WEB_SYSTEMATICSPANEL_HPP_INCLUDE
