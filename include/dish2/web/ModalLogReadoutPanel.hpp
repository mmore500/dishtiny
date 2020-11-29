#pragma once
#ifndef DISH2_WEB_MODALLOGREADOUTPANEL_HPP_INCLUDE
#define DISH2_WEB_MODALLOGREADOUTPANEL_HPP_INCLUDE

#include "../../../third-party/Empirical/source/web/Document.h"

#include "../prefab/ModalLogReadout.hpp"

namespace dish2 {

class ModalLogReadoutPanel {

  emp::web::Document panel{ "modal-log-readout-panel" };

  dish2::ModalLogReadout readout;

public:

  ModalLogReadoutPanel() { panel << (emp::web::Div) readout; }

};

} // namespace dish2

#endif // #ifndef DISH2_WEB_MODALLOGREADOUTPANEL_HPP_INCLUDE
