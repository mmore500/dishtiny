#include "Catch/single_include/catch2/catch.hpp"
#include "Empirical/include/emp/web/Canvas.hpp"
#include "Empirical/include/emp/web/Document.hpp"
#include "Empirical/include/emp/web/NodeDomShim.hpp"

#include "dish2/viz/artists/DummyArtist.hpp"

const emp::web::NodeDomShim shim;

emp::web::Document emp_base{ "emp_base" };

TEST_CASE("Test DummyArtist") {

  emp::web::Canvas canvas(500, 500);
  emp_base << canvas;

  dish2::DummyArtist artist;

  artist.Draw( canvas );

  canvas.SavePNG( "DummyArtist.png" );

  emscripten_run_script(R"(
    var exec = require('child_process').exec;

    setTimeout(function() { exec(
      'cmp -s DummyArtist.png assets/DummyArtist.png',
      function(err, stdout, stderr) {
        if ( err ) {
          console.log( 'mismatch between generated and expected pngs' );
          process.exit( err.code );
        }
      }
    ) }, 1000);

  )");

}
