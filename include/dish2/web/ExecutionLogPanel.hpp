#pragma once
#ifndef DISH2_WEB_EXECUTIONLOGPANEL_HPP_INCLUDE
#define DISH2_WEB_EXECUTIONLOGPANEL_HPP_INCLUDE

#include <emscripten.h>
#include <emscripten/threading.h>

#include "../../../third-party/Empirical/include/emp/web/Document.hpp"
#include "../../../third-party/Empirical/include/emp/web/JSWrap.hpp"

#include "../debug/log_level.hpp"
#include "../prefab/LogEventReadout.hpp"
#include "../prefab/LogStackReadout.hpp"

namespace dish2 {

class ExecutionLogPanel {

  emp::web::Document stack_panel{ "execution-log-stack-panel" };

  emp::web::Document events_panel{ "execution-log-events-panel" };

  dish2::LogStackReadout log_stack_readout;

  dish2::LogEventReadout log_events_readout;


public:

  ExecutionLogPanel() {

    // add callback on opening the panel that increases the log level
    // or on closing the panel resets the log level
    {
      const size_t log_level_fun = emp::JSWrap( [](){
        dish2::log_level = 10;
      } );
      const size_t log_release_fun = emp::JSWrap( [this](){
        Clear();
        // adapted from https://github.com/emscripten-core/emscripten/blob/2bca083cbbd5a4133db61fbd74d04f7feecfa907/tests/pthread/test_futex_wake_all.cpp
        emscripten_atomic_store_u32(
          &dish2::log_futex_val,  dish2::log_futex_release
        );
        emscripten_futex_wake( &dish2::log_futex_val, 1 );
      } );

      MAIN_THREAD_EM_ASM({
        $('#execution-log-panel-button').on('click', function() {
          if ( !$('#execution-log-panel-button').hasClass('active') ) {
            empCppCallback($0);
          } else {
            // remove all open detail popovers
            $("[data-toggle='popover']").popover('dispose');
            empDoCppCallback($1);
          }
        });
      }, log_level_fun, log_release_fun);
    }

    // add callback to stepper button that wakes log futex
    {
      const size_t fun_id = emp::JSWrap( [](){
        // adapted from https://github.com/emscripten-core/emscripten/blob/2bca083cbbd5a4133db61fbd74d04f7feecfa907/tests/pthread/test_futex_wake_all.cpp
        const int step = MAIN_THREAD_EM_ASM_INT({
          return $('#execution-log-panel-step').val();
        });
        emscripten_atomic_store_u32( &dish2::log_futex_val, step );
        emscripten_futex_wake( &dish2::log_futex_val, 1 );
      } );
      MAIN_THREAD_EM_ASM({
        $('#execution-log-panel-stepper').on('click', function() {
          // remove all open detail popovers
          $("[data-toggle='popover']").popover('dispose');
          // using empDoCppCallback ensures that C++ callback is processed on
          // browser thread, not web worker (which is busy spin waiting)
          empDoCppCallback($0);
        });
      }, fun_id);
    }

    stack_panel.SetAttr(
      "class",
      "list-group-item"
    ) << (emp::web::Div) log_stack_readout;

    events_panel.SetAttr(
      "class",
      "list-group-item",
      "style", // workaround for css not applying
      "max-height: 40vh; display: flex;"
    ) << (emp::web::Div) log_events_readout;

  }

  void Clear() {
    log_stack_readout.Clear();
    log_stack_readout.Redraw();
    log_events_readout.Clear();
    log_events_readout.Redraw();
  }

};

} // namespace dish2

#endif // #ifndef DISH2_WEB_EXECUTIONLOGPANEL_HPP_INCLUDE
