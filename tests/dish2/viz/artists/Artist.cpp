#include "Catch/single_include/catch2/catch.hpp"
#include "conduit/include/uitsl/debug/compare_files.hpp"
#include "Empirical/include/emp/web/Canvas.hpp"
#include "Empirical/include/emp/web/Document.hpp"
#include "Empirical/include/emp/web/NodeDomShim.hpp"

#include "dish2/config/TemporaryConfigOverride.hpp"
#include "dish2/viz/artists/Artist.hpp"
#include "dish2/viz/border_colormaps/DummyBorderColorMap.hpp"
#include "dish2/viz/fill_colormaps/DummyFillColorMap.hpp"
#include "dish2/viz/getters/DummyGetter.hpp"
#include "dish2/viz/renderers/CellBorderRenderer.hpp"
#include "dish2/viz/renderers/CellFillRenderer.hpp"

const emp::web::NodeDomShim shim;

emp::web::Document emp_base{ "emp_base" };

TEST_CASE("Test Artist") {

  const dish2::TemporaryConfigOverride config_override( "N_CELLS", 100 );

  emp::web::Canvas canvas(500, 500);
  emp_base << canvas;

  using getter_t = dish2::DummyGetter<size_t>;

  using border_renderer_t = dish2::CellBorderRenderer<
    dish2::DummyBorderColorMap<>,
    getter_t
  >;

  using fill_renderer_t = dish2::CellFillRenderer<
    dish2::DummyFillColorMap<255, 0, 0>,
    getter_t
  >;

  dish2::Artist<fill_renderer_t, border_renderer_t> artist;

  artist.Draw( canvas );

  canvas.SavePNG( "Artist.png" );

  emscripten_run_script(R"(
    var exec = require('child_process').exec;

    setTimeout(function() { exec(
      'cmp -s Artist.png assets/Artist.png',
      function(err, stdout, stderr) {
        if ( err ) {
          console.log( 'mismatch between generated and expected pngs' );
          process.exit( err.code );
        }
      }
    ) }, 1000);

  )");

}
