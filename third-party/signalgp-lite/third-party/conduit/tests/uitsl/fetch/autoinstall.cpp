#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/debug/compare_files.hpp"
#include "uitsl/fetch/autoinstall.hpp"
#include "uitsl/polyfill/filesystem.hpp"

TEST_CASE("should install .tar.gz", "[nproc:1]") {

  std::filesystem::remove( "rick.txt" );
  REQUIRE( !std::filesystem::exists( "rick.txt" ) );

  REQUIRE(
    "rick.tar" ==
    uitsl::autoinstall( "https://raw.githubusercontent.com/mmore500/conduit/master/tests/uitsl/fetch/assets/rick.tar.gz" )
  );

  REQUIRE( std::filesystem::exists( "rick.txt" ) );
  REQUIRE( uitsl::compare_files( "assets/rick.txt", "rick.txt" ) );

  std::filesystem::remove( "rick.txt" );
  REQUIRE( !std::filesystem::exists( "rick.txt" ) );

}

TEST_CASE("should install .tar", "[nproc:1]") {

  std::filesystem::remove( "rick.txt" );
  REQUIRE( !std::filesystem::exists( "rick.txt" ) );

  REQUIRE(
    "rick.tar" ==
    uitsl::autoinstall( "https://raw.githubusercontent.com/mmore500/conduit/master/tests/uitsl/fetch/assets/rick.tar" )
  );

  REQUIRE( std::filesystem::exists( "rick.txt" ) );
  REQUIRE( uitsl::compare_files( "assets/rick.txt", "rick.txt" ) );

  std::filesystem::remove( "rick.txt" );
  REQUIRE( !std::filesystem::exists( "rick.txt" ) );

}

TEST_CASE("should install .gz", "[nproc:1]") {

  std::filesystem::remove( "twas-brillig-and-the-slithy-toves-did-gyre-and-gimble-in-the-wabe-all-mimsy-were-the-borogoves-and-the-mome-raths-outgrabe-beware-the-jabberwock-my-son" );
  REQUIRE( !std::filesystem::exists( "twas-brillig-and-the-slithy-toves-did-gyre-and-gimble-in-the-wabe-all-mimsy-were-the-borogoves-and-the-mome-raths-outgrabe-beware-the-jabberwock-my-son" ) );

  REQUIRE(
    "twas-brillig-and-the-slithy-toves-did-gyre-and-gimble-in-the-wabe-all-mimsy-were-the-borogoves-and-the-mome-raths-outgrabe-beware-the-jabberwock-my-son" ==
    uitsl::autoinstall( "https://raw.githubusercontent.com/mmore500/conduit/master/tests/uitsl/fetch/assets/twas-brillig-and-the-slithy-toves-did-gyre-and-gimble-in-the-wabe-all-mimsy-were-the-borogoves-and-the-mome-raths-outgrabe-beware-the-jabberwock-my-son.gz" )
  );

  REQUIRE( std::filesystem::exists( "twas-brillig-and-the-slithy-toves-did-gyre-and-gimble-in-the-wabe-all-mimsy-were-the-borogoves-and-the-mome-raths-outgrabe-beware-the-jabberwock-my-son" ) );
  REQUIRE( uitsl::compare_files( "assets/twas-brillig-and-the-slithy-toves-did-gyre-and-gimble-in-the-wabe-all-mimsy-were-the-borogoves-and-the-mome-raths-outgrabe-beware-the-jabberwock-my-son", "twas-brillig-and-the-slithy-toves-did-gyre-and-gimble-in-the-wabe-all-mimsy-were-the-borogoves-and-the-mome-raths-outgrabe-beware-the-jabberwock-my-son" ) );

  std::filesystem::remove( "twas-brillig-and-the-slithy-toves-did-gyre-and-gimble-in-the-wabe-all-mimsy-were-the-borogoves-and-the-mome-raths-outgrabe-beware-the-jabberwock-my-son" );
  REQUIRE( !std::filesystem::exists( "twas-brillig-and-the-slithy-toves-did-gyre-and-gimble-in-the-wabe-all-mimsy-were-the-borogoves-and-the-mome-raths-outgrabe-beware-the-jabberwock-my-son" ) );

}

TEST_CASE("should install plain text file", "[nproc:1]") {

  std::filesystem::remove( "rick.txt" );
  REQUIRE( !std::filesystem::exists( "rick.txt" ) );

  REQUIRE(
    "rick.txt" ==
    uitsl::autoinstall( "https://raw.githubusercontent.com/mmore500/conduit/master/tests/uitsl/fetch/assets/rick.txt" )
  );

  REQUIRE( std::filesystem::exists( "rick.txt" ) );
  REQUIRE( uitsl::compare_files( "assets/rick.txt", "rick.txt" ) );

  std::filesystem::remove( "rick.txt" );
  REQUIRE( !std::filesystem::exists( "rick.txt" ) );

}

TEST_CASE("should be compatible with osf", "[nproc:1]") {

  std::filesystem::remove( "run_type=aggregate+time_type=cpu_time+ext=.csv" );
  REQUIRE( !std::filesystem::exists(
    "run_type=aggregate+time_type=cpu_time+ext=.csv" )
  );

  REQUIRE(
    "run_type=aggregate+time_type=cpu_time+ext=.csv" ==
    uitsl::autoinstall( "https://osf.io/d23bq/download" )
  );

  REQUIRE( std::filesystem::exists(
    "run_type=aggregate+time_type=cpu_time+ext=.csv"
  ) );
  REQUIRE( uitsl::compare_files(
    "assets/run_type=aggregate+time_type=cpu_time+ext=.csv",
    "run_type=aggregate+time_type=cpu_time+ext=.csv" )
  );

  std::filesystem::remove( "run_type=aggregate+time_type=cpu_time+ext=.csv" );
  REQUIRE( !std::filesystem::exists(
    "run_type=aggregate+time_type=cpu_time+ext=.csv"
  ) );

}
