#define DISH2_LOG_ENABLE

#include <emscripten.h>

#include "conduit/include/uitsl/polyfill/ompi_mpi_comm_world.hpp"
#include "Empirical/include/emp/config/ArgManager.hpp"
#include "Empirical/include/emp/web/UrlParams.hpp"

#include "dish2/config/cfg.hpp"
#include "dish2/config/make_arg_specs.hpp"
#include "dish2/config/setup.hpp"
#include "dish2/config/thread_idx.hpp"
#include "dish2/spec/print_spec.hpp"
#include "dish2/spec/Spec.hpp"
#include "dish2/web/WebInterface.hpp"
#include "dish2/world/ProcWorld.hpp"

using Spec = DISH2_SPEC;

thread_local dish2::WebInterface<Spec>* interface;

int main() {

  dish2::thread_idx = 0;

  dish2::setup( emp::ArgManager{
    emp::web::GetUrlParams(), dish2::make_arg_specs<Spec>()
  } );
  dish2::print_spec<Spec>();

  interface = new dish2::WebInterface<Spec>;

  // set up web interface
  interface->Redraw();

  emscripten_set_main_loop([](){}, 1, true);

  // once we're done setting up, turn off the loading modal
  // MAIN_THREAD_EM_ASM({ $('.modal').modal('hide'); });

  return 0;
}
