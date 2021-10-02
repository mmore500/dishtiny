#include "Catch/single_include/catch2/catch.hpp"
#include "conduit/include/uitsl/polyfill/ompi_mpi_comm_world.hpp"
#include "Empirical/include/emp/base/vector.hpp"
#include "Empirical/include/emp/web/Document.hpp"
#include "Empirical/include/emp/web/NodeDomShim.hpp"

#include "dish2/config/TemporaryThreadIdxOverride.hpp"
#include "dish2/viz/artists/DummyArtist.hpp"
#include "dish2/web/ViewerPanel.hpp"
#include "dish2/world/ProcWorld.hpp"
#include "dish2/world/ThreadWorld.hpp"

using Spec = dish2::Spec_default;

const dish2::TemporaryThreadIdxOverride override{ 0 };

const emp::web::NodeDomShim shim{{ "grid_viewer", "view_selctor" }};

TEST_CASE("Test ViewerPanel") {

  auto thread_world = dish2::ProcWorld<Spec>{}.MakeThreadWorld();

  new dish2::ViewerPanel{ thread_world };

}
