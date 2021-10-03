#define DISH2_LOG_ENABLE

#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include "conduit/include/uitsl/polyfill/ompi_mpi_comm_world.hpp"
#include "Empirical/include/emp/config/ArgManager.hpp"
#include "Empirical/include/emp/web/init.hpp"
#include "Empirical/include/emp/web/UrlParams.hpp"

#include "dish2/config/cfg.hpp"
#include "dish2/config/make_arg_specs.hpp"
#include "dish2/config/setup.hpp"
#include "dish2/config/TemporaryThreadIdxOverride.hpp"
#include "dish2/spec/print_spec.hpp"
#include "dish2/spec/Spec.hpp"
#include "dish2/utility/print_js_stacktrace.hpp"
#include "dish2/web/WebInterface.hpp"
#include "dish2/world/ProcWorld.hpp"

using Spec = DISH2_SPEC;

// permanent in this case
dish2::TemporaryThreadIdxOverride override{ 0 };

// intentionally leaked
thread_local dish2::WebInterface<Spec>* interface;

void do_main() {

  dish2::setup<Spec>( emp::ArgManager{
    emp::web::GetUrlParams(), dish2::make_arg_specs<Spec>()
  } );
  dish2::print_spec<Spec>();

  // set up web interface
  interface = new dish2::WebInterface<Spec>;
  interface->Redraw();

  emscripten_set_main_loop([](){}, 1, true);

  // once we're done setting up, turn off the loading modal
  // MAIN_THREAD_EM_ASM({ $('.modal').modal('hide'); });

}

int main() {

  emp::Initialize();

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
