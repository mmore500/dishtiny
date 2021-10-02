#include "Catch/single_include/catch2/catch.hpp"
#include "Empirical/include/emp/web/Canvas.hpp"
#include "Empirical/include/emp/web/Document.hpp"
#include "Empirical/include/emp/web/NodeDomShim.hpp"

#include "dish2/viz/border_colormaps/DummyBorderColorMap.hpp"
#include "dish2/viz/getters/DummyGetter.hpp"
#include "dish2/viz/renderers/CellBorderRenderer.hpp"

const emp::web::NodeDomShim shim;

emp::web::Document emp_base{ "emp_base" };

TEST_CASE("Test CellBorderRenderer") {

  emp::web::Canvas canvas(500, 500);
  emp_base << canvas;

  using getter_t = dish2::DummyGetter<size_t>;

  dish2::CellBorderRenderer<
    dish2::DummyBorderColorMap<255, 0, 0>,
    getter_t
  > renderer{ getter_t{} };

  renderer.Render( canvas );

  canvas.SavePNG( "CellBorderRenderer.png" );

  emscripten_run_script(R"(
    var exec = require('child_process').exec;

    setTimeout(function() { exec(
      'cmp -s CellBorderRenderer.png assets/CellBorderRenderer.png',
      function(err, stdout, stderr) {
        if ( err ) {
          console.log( 'mismatch between generated and expected pngs' );
          process.exit( err.code );
        }
      }
    ) }, 1000);

  )");

}
