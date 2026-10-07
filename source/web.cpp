#define DISH2_LOG_ENABLE

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <new>
#include <stdexcept>
#include <string>
#include <unordered_map>

#include "conduit/include/uitsl/polyfill/ompi_mpi_comm_world.hpp"
#include "Empirical/include/emp/config/ArgManager.hpp"
#include "Empirical/include/emp/web/init.hpp"
#include "Empirical/include/emp/web/UrlParams.hpp"

#include "dish2/config/cfg.hpp"
#include "dish2/config/make_arg_specs.hpp"
#include "dish2/config/setup.hpp"
#include "dish2/config/TemporaryThreadIdxOverride.hpp"
#include "dish2/debug/log_tee.hpp"
#include "dish2/spec/print_spec.hpp"
#include "dish2/spec/Spec.hpp"
#include "dish2/utility/print_js_stacktrace.hpp"
#include "dish2/web/click_elements.hpp"
#include "dish2/web/WebInterface.hpp"
#include "dish2/world/ProcWorld.hpp"

using Spec = DISH2_SPEC;

// these ptrs intentionally leaked
thread_local dish2::TemporaryThreadIdxOverride* override;
thread_local dish2::WebInterface<Spec>* interface;

void do_main() {

  // TemporaryThreadIdxOverride is permanent in this case
  // must be performed here due to initialization order issues
  override = new dish2::TemporaryThreadIdxOverride(0);

  // web-only argument: ids of elements to click once the page is set up
  emp::vector<std::string> click_ids;
  auto specs = dish2::make_arg_specs<Spec>();
  specs.merge( std::unordered_map<std::string,emp::ArgSpec>{
    {"click", emp::ArgSpec(
      std::numeric_limits<size_t>::max(), // most quota
      1, // least quota
      "ids of elements to click after setup", // description
      {}, // aliases
      [&click_ids](const emp::optional<emp::vector<std::string>>& args){
        if ( args ) click_ids.insert(
          click_ids.end(), args->begin(), args->end()
        );
      }, // callback
      false, // gobble_flags
      true // flatten
    )}
  } );

  dish2::setup<Spec>( emp::ArgManager{ emp::web::GetUrlParams(), specs } );
  dish2::print_spec<Spec>();

  // set up web interface
  interface = new dish2::WebInterface<Spec>;
  interface->Redraw();

  dish2::click_elements( click_ids );


  // once we're done setting up, turn off the loading modal
  // ... disabled until loading modal is reactivated
  // MAIN_THREAD_EM_ASM({ $('.modal').modal('hide'); });

  dish2::log_tee << "web viewer load complete" << '\n';

  // believe (?) this was added to prevent Emscripten from calling destructors
  // on globals once main() exits... not sure if it actually does anything
  // @MAM 10-2021 looks like this is necessary
  // ... at when using pthreads with puppeteer (didn't test w/o pthreads)
  emscripten_set_main_loop([](){}, 1, true);

}

int main() {

  emp::Initialize();

  // set_new_handler is specific to failed allocation
  std::set_new_handler( [](){
    std::fputs( "out of memory: wasm heap exhausted\n", stderr );
    std::abort();
  } );

  // called when exception handling fails
  std::set_terminate( dish2::print_js_stacktrace );

  try {
    do_main();
  } catch(const std::exception &e) {
    std::cout << "Uncaught exception: " << e.what() << '\n';
  } catch(...) {
    std::cout << "Uncaught exception: unknown" << '\n';
  }

  return 0;

}
