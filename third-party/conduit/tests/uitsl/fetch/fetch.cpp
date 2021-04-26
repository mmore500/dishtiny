#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/debug/compare_files.hpp"
#include "uitsl/fetch/fetch.hpp"

TEST_CASE("fetch text data", "[nproc:1]") {

  REQUIRE( uitsl::compare_files(
    "assets/TheWorldWideWebProject.html",
    uitsl::fetch( "http://info.cern.ch/hypertext/WWW/TheProject.html" )
  ) );

}

TEST_CASE("fetch binary data", "[nproc:1]") {

  REQUIRE( uitsl::compare_files(
    "assets/jabberwocky.tar.gz",
    uitsl::fetch( "https://raw.githubusercontent.com/mmore500/conduit/master/tests/uitsl/fetch/assets/jabberwocky.tar.gz" )
  ) );

}

TEST_CASE("fetch with redirect", "[nproc:1]") {

  REQUIRE( uitsl::compare_files(
    "assets/run_type=aggregate+time_type=cpu_time+ext=.csv",
    uitsl::fetch( "https://osf.io/d23bq/download" )
  ) );

}
