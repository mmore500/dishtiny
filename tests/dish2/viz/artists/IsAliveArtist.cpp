#include "Catch/single_include/catch2/catch.hpp"
#include "conduit/include/uitsl/debug/compare_files.hpp"
#include "conduit/include/uitsl/polyfill/ompi_mpi_comm_world.hpp"
#include "Empirical/include/emp/web/Canvas.hpp"
#include "Empirical/include/emp/web/Document.hpp"
#include "Empirical/include/emp/web/NodeDomShim.hpp"

#include "dish2/config/TemporaryConfigOverride.hpp"
#include "dish2/config/TemporaryThreadIdxOverride.hpp"
#include "dish2/spec/Spec.hpp"
#include "dish2/viz/artists/IsAliveArtist.hpp"
#include "dish2/viz/getters/IsAliveGetter.hpp"
#include "dish2/viz/renderers/CellBorderRenderer.hpp"
#include "dish2/viz/renderers/CellFillRenderer.hpp"
#include "dish2/world/ProcWorld.hpp"
#include "dish2/world/ThreadWorld.hpp"

using Spec = dish2::Spec_default;

const dish2::TemporaryConfigOverride config_override( "N_CELLS", 100 );
const dish2::TemporaryThreadIdxOverride override{ 0 };

const emp::web::NodeDomShim shim;

emp::web::Document emp_base{ "emp_base" };

TEST_CASE("Test IsAliveArtist") {

  emp::web::Canvas canvas(500, 500);
  emp_base << canvas;

  auto tw = dish2::ProcWorld<Spec>{}.MakeThreadWorld();

  dish2::IsAliveArtist<> artist{ tw };

  artist.Draw( canvas );

  canvas.SavePNG( "IsAliveArtist.png" );

  emscripten_run_script(R"(
    var exec = require('child_process').exec;

    setTimeout(function() { exec(
      'cmp -s IsAliveArtist.png assets/IsAliveArtist.png',
      function(err, stdout, stderr) {
        if ( err ) {
          console.log( 'mismatch between generated and expected pngs' );
          process.exit( err.code );
        }
      }
    ) }, 1000);

  )");

}
