#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/debug/compare_files.hpp"
#include "uitsl/fetch/inflate.hpp"

TEST_CASE("inflate empty.tar.gz", "[nproc:1]") {

  const auto temp_path{ uitsl::inflate( "assets/empty.tar.gz" ) };

  REQUIRE( uitsl::compare_files( "assets/empty.tar", temp_path ) );

}

TEST_CASE("inflate jabberwocky.tar.gz", "[nproc:1]") {

  const auto temp_path{ uitsl::inflate( "assets/jabberwocky.tar.gz" ) };

  REQUIRE( uitsl::compare_files( "assets/jabberwocky.tar", temp_path ) );

}

TEST_CASE("inflate nested.tar.gz", "[nproc:1]") {

  const auto temp_path{ uitsl::inflate( "assets/nested.tar.gz" ) };

  REQUIRE( uitsl::compare_files( "assets/nested.tar", temp_path ) );

}

TEST_CASE("inflate rick.tar.gz", "[nproc:1]") {

  const auto temp_path{ uitsl::inflate( "assets/rick.tar.gz" ) };

  REQUIRE( uitsl::compare_files( "assets/rick.tar", temp_path ) );

}

TEST_CASE("inflate twas-brillig-and-the-slithy-toves-did-gyre-and-gimble-in-the-wabe-all-mimsy-were-the-borogoves-and-the-mome-raths-outgrabe-beware-the-jabberwock-my-son.gz", "[nproc:1]") {

  const auto temp_path{ uitsl::inflate( "assets/twas-brillig-and-the-slithy-toves-did-gyre-and-gimble-in-the-wabe-all-mimsy-were-the-borogoves-and-the-mome-raths-outgrabe-beware-the-jabberwock-my-son.gz" ) };

  REQUIRE( uitsl::compare_files( "assets/twas-brillig-and-the-slithy-toves-did-gyre-and-gimble-in-the-wabe-all-mimsy-were-the-borogoves-and-the-mome-raths-outgrabe-beware-the-jabberwock-my-son", temp_path ) );

}

TEST_CASE("inflate zen.tar.gz", "[nproc:1]") {

  const auto temp_path{ uitsl::inflate( "assets/zen.tar.gz" ) };

  REQUIRE( uitsl::compare_files( "assets/zen.tar", temp_path ) );

}

TEST_CASE("inflate a=genome+criteria=arbitrary_over_all_roots+morph=wildtype+proc=0+series=16000+stint=0+thread=1+variation=master+ext=.json.gz", "[nproc:1]") {

  const auto temp_path{ uitsl::inflate( "assets/a=genome+criteria=arbitrary_over_all_roots+morph=wildtype+proc=0+series=16000+stint=0+thread=1+variation=master+ext=.json.gz" ) };

  REQUIRE( uitsl::compare_files( "assets/a=genome+criteria=arbitrary_over_all_roots+morph=wildtype+proc=0+series=16000+stint=0+thread=1+variation=master+ext=.json", temp_path ) );

}
