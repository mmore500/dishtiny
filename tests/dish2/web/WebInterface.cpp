#define CATCH_CONFIG_MAIN

#include "Catch/single_include/catch2/catch.hpp"
#include "conduit/include/uitsl/polyfill/ompi_mpi_comm_world.hpp"
#include "Empirical/source/base/vector.h"
#include "Empirical/source/web/Document.h"
#include "Empirical/source/web/NodeDomShim.h"

#include "dish2/viz/artists/DummyArtist.hpp"
#include "dish2/web/WebInterface.hpp"

const emp::web::NodeDomShim shim{{
  "emp_button_dash", "grid_viewer", "view_selctor"
}};

TEST_CASE("Test WebInterface") {

  new dish2::WebInterface;

}
