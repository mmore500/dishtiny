#define CATCH_CONFIG_MAIN

#include "Catch/single_include/catch2/catch.hpp"
#include "conduit/include/uitsl/debug/compare_files.hpp"
#include "conduit/include/uitsl/polyfill/ompi_mpi_comm_world.hpp"
#include "Empirical/source/web/Canvas.h"
#include "Empirical/source/web/Document.h"
#include "Empirical/source/web/NodeDomShim.h"

#include "dish2/viz/artists/PhylogeneticRootArtist.hpp"
#include "dish2/viz/getters/PhylogeneticRootGetter.hpp"
#include "dish2/viz/renderers/CellFillRenderer.hpp"
#include "dish2/viz/renderers/CellBorderRenderer.hpp"
#include "dish2/world/ProcWorld.hpp"
#include "dish2/world/ThreadWorld.hpp"
#include "dish2/spec/Spec.hpp"

const emp::web::NodeDomShim shim;

emp::web::Document emp_base{ "emp_base" };

TEST_CASE("Test PhylogeneticRootArtist") {

  emp::web::Canvas canvas(500, 500);
  emp_base << canvas;

  auto tw = dish2::ProcWorld<dish2::Spec>{}.MakeThreadWorld(0);

  dish2::PhylogeneticRootArtist<> artist{ tw };

  artist.Draw( canvas );

  canvas.SavePNG( "PhylogeneticRootArtist.png" );

  emscripten_run_script(R"(
    var exec = require('child_process').exec;

    setTimeout(function() { exec(
      'cmp -s PhylogeneticRootArtist.png assets/PhylogeneticRootArtist.png',
      function(err, stdout, stderr) {
        if ( err ) {
          console.log( 'mismatch between generated and expected pngs' );
          process.exit( err.code );
        }
      }
    ) }, 1000);

  )");

}
