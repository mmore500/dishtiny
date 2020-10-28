#define CATCH_CONFIG_MAIN

#include "Catch/single_include/catch2/catch.hpp"
#include "conduit/include/uitsl/polyfill/ompi_mpi_comm_world.hpp"
#include "Empirical/source/base/vector.h"
#include "Empirical/source/web/Document.h"
#include "Empirical/source/web/NodeDomShim.h"

#include "dish2/viz/artists/DummyArtist.hpp"
#include "dish2/web/ViewerPanel.hpp"
#include "dish2/world/ProcWorld.hpp"
#include "dish2/world/ThreadWorld.hpp"

const emp::web::NodeDomShim shim{{ "grid_viewer", "view_selctor" }};

TEST_CASE("Test ViewerPanel") {

  auto thread_world = dish2::ProcWorld<dish2::Spec>{}.MakeThreadWorld(0);

  new dish2::ViewerPanel{ thread_world };

}
