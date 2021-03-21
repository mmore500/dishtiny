#pragma once
#ifndef DISH2_WEB_WEBINTERFACE_HPP_INCLUDE
#define DISH2_WEB_WEBINTERFACE_HPP_INCLUDE

#include "../load/load_world.hpp"
#include "../world/ProcWorld.hpp"
#include "../world/ThreadWorld.hpp"

#include "ControlPanel.hpp"
#include "ExecutionLogPanel.hpp"
#include "ModalLogReadoutPanel.hpp"
#include "PrevalentGenotypePanel.hpp"
#include "SystematicsPanel.hpp"
#include "ViewerPanel.hpp"

namespace dish2 {

template< typename Spec >
class WebInterface {

  dish2::ThreadWorld<Spec> thread_world{
    dish2::ProcWorld<Spec>{}.MakeThreadWorld()
  };

  dish2::ControlPanel control_panel{
    [this](const bool render_toggle, const size_t render_freq){
      thread_world.template Update<false>();
      Redraw( render_toggle, render_freq );
      return thread_world.GetUpdate();
    }
  };
  dish2::ExecutionLogPanel execution_log_panel;
  dish2::ModalLogReadoutPanel modal_log_readout_panel;
  dish2::PrevalentGenotypePanel<Spec> prevalent_genotype_panel{ thread_world };
  dish2::SystematicsPanel<Spec> systematics_panel{ thread_world };
  dish2::ViewerPanel<Spec> view_panel{ thread_world };

public:

  WebInterface() {
    dish2::load_world<Spec>( thread_world );
  }

  void Redraw( const bool render_toggle=true, const size_t render_freq=1 ) {

    emp_assert( render_freq );
    if (
      render_toggle && thread_world.GetUpdate() % render_freq == 0
    ) {
      view_panel.Redraw();
      systematics_panel.Redraw();
    };

  }

};

} // namespace dish2

#endif // #ifndef DISH2_WEB_WEBINTERFACE_HPP_INCLUDE
