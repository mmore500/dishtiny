#define CATCH_CONFIG_MAIN

#include "Catch/single_include/catch2/catch.hpp"
#include "Empirical/source/web/Canvas.h"
#include "Empirical/source/web/Document.h"
#include "Empirical/source/web/NodeDomShim.h"

#include "dish2/web/border_colormaps/DummyBorderColorMap.hpp"
#include "dish2/web/getters/DummyGetter.hpp"
#include "dish2/web/renderers/CellBorderRenderer.hpp"

const emp::web::NodeDomShim shim;

emp::web::Document emp_base{ "emp_base" };

TEST_CASE("Test CellBorderRenderer") {

  emp::web::Canvas canvas(500, 500);
  emp_base << canvas;

  using getter_t = dish2::DummyGetter<size_t>;
  const getter_t getter;

  dish2::CellBorderRenderer<
    dish2::DummyBorderColorMap<getter_t, 255, 0, 0>,
    getter_t
  > renderer{ {} };

  renderer.Render( canvas );

  canvas.SavePNG( "CellBorderRenderer.png" );

  emscripten_run_script(R"(
    var exec = require('child_process').exec;

    setTimeout(function() { exec(
      'cmp -s CellBorderRenderer.png assets/CellBorderRenderer.png',
      function(err, stdout, stderr) {
        if ( err ) { process.exit( err.code ); }
      }
    ) }, 1000);

  )");

}
