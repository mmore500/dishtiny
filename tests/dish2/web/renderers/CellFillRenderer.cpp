#define CATCH_CONFIG_MAIN

#include "Catch/single_include/catch2/catch.hpp"
#include "Empirical/source/web/Canvas.h"
#include "Empirical/source/web/Document.h"
#include "Empirical/source/web/NodeDomShim.h"

#include "dish2/web/fill_colormaps/DummyFillColorMap.hpp"
#include "dish2/web/getters/DummyGetter.hpp"
#include "dish2/web/renderers/CellFillRenderer.hpp"

const emp::web::NodeDomShim shim;

emp::web::Document emp_base{ "emp_base" };

TEST_CASE("Test CellFillRenderer") {
  emp::web::Canvas canvas(500, 500);
  emp_base << canvas;

  using getter_t = dish2::DummyGetter<size_t>;
  const getter_t getter;

  dish2::CellFillRenderer<
    dish2::DummyFillColorMap<getter_t, 255, 0, 0>,
    getter_t
  > renderer{ {} };

  renderer.Render( canvas );

  canvas.SavePNG( "CellFillRenderer.png" );

  emscripten_run_script(R"(
    var exec = require('child_process').exec;

    setTimeout(function() { exec(
      'cmp -s CellFillRenderer.png assets/CellFillRenderer.png',
      function(err, stdout, stderr) {
        if ( err ) { process.exit( err.code ); }
      }
    ) }, 1000);

  )");

}
