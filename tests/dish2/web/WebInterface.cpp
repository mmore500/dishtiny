#include "Catch/single_include/catch2/catch.hpp"
#include "conduit/include/uitsl/polyfill/ompi_mpi_comm_world.hpp"
#include "Empirical/include/emp/base/vector.hpp"
#include "Empirical/include/emp/web/Document.hpp"
#include "Empirical/include/emp/web/NodeDomShim.hpp"

#include "dish2/viz/artists/DummyArtist.hpp"
#include "dish2/web/WebInterface.hpp"

const emp::web::NodeDomShim shim{{
  "emp_button_dash", "grid_viewer", "view_selctor"
}};

TEST_CASE("Test WebInterface") {

  new dish2::WebInterface;

}
