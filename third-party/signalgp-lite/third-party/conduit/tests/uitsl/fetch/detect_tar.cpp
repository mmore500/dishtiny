#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/fetch/detect_tar.hpp"

TEST_CASE("should detect tar", "[nproc:1]") {

  REQUIRE( uitsl::detect_tar( "assets/empty.tar" ) );
  REQUIRE( uitsl::detect_tar( "assets/jabberwocky.tar" ) );
  REQUIRE( uitsl::detect_tar( "assets/nested.tar" ) );
  REQUIRE( uitsl::detect_tar( "assets/rick.tar" ) );
  REQUIRE( uitsl::detect_tar( "assets/zen.tar" ) );

}

TEST_CASE("shouldn't detect tar", "[nproc:1]") {

  REQUIRE( !uitsl::detect_tar( "assets/beautiful-is-better-than-ugly-explicit-is-better-than-implicit-simple-is-better-than-complex-complex-is-better-than-complicated-flat-is-better-than-nested" ) );
  REQUIRE( !uitsl::detect_tar( "assets/empty.tar.gz" ) );
  REQUIRE( !uitsl::detect_tar( "assets/jabberwocky.tar.gz" ) );
  REQUIRE( !uitsl::detect_tar( "assets/nested.tar.gz" ) );
  REQUIRE( !uitsl::detect_tar( "assets/rick.tar.gz" ) );
  REQUIRE( !uitsl::detect_tar( "assets/rick.txt" ) );
  REQUIRE( !uitsl::detect_tar( "assets/TheWorldWideWebProject.html" ) );
  REQUIRE( !uitsl::detect_tar( "assets/twas-brillig-and-the-slithy-toves-did-gyre-and-gimble-in-the-wabe-all-mimsy-were-the-borogoves-and-the-mome-raths-outgrabe-beware-the-jabberwock-my-son" ) );
  REQUIRE( !uitsl::detect_tar( "assets/twas-brillig-and-the-slithy-toves-did-gyre-and-gimble-in-the-wabe-all-mimsy-were-the-borogoves-and-the-mome-raths-outgrabe-beware-the-jabberwock-my-son.gz" ) );
  REQUIRE( !uitsl::detect_tar( "assets/zen.tar.gz" ) );

}
