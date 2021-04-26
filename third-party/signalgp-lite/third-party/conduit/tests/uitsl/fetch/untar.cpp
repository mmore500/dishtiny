#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/debug/compare_files.hpp"
#include "uitsl/fetch/untar.hpp"

TEST_CASE("untar file", "[nproc:1]") {

  REQUIRE( uitsl::untar( "assets/rick.tar" ) );

  REQUIRE( uitsl::compare_files( "assets/rick.txt", "rick.txt" ) );

}

TEST_CASE("untar nested directory", "[nproc:1]") {

  REQUIRE( uitsl::untar( "assets/nested.tar" ) );

  REQUIRE( uitsl::compare_files(
    "assets/rick.txt", "never/gonna/let/you/down/rick.txt"
  ) );

}

TEST_CASE("untar empty directory", "[nproc:1]") {

  REQUIRE( uitsl::untar( "assets/empty.tar" ) );

  REQUIRE( std::filesystem::exists( "empty/also_empty" ) );

}

TEST_CASE("untar long filename", "[nproc:1]") {

  REQUIRE( uitsl::untar( "assets/jabberwocky.tar" ) );

  REQUIRE( uitsl::compare_files(
    "assets/twas-brillig-and-the-slithy-toves-did-gyre-and-gimble-in-the-wabe-all-mimsy-were-the-borogoves-and-the-mome-raths-outgrabe-beware-the-jabberwock-my-son",
    "twas-brillig-and-the-slithy-toves-did-gyre-and-gimble-in-the-wabe-all-mimsy-were-the-borogoves-and-the-mome-raths-outgrabe-beware-the-jabberwock-my-son"
  ) );

}

TEST_CASE("untar long filename and long directory name", "[nproc:1]") {

  REQUIRE( uitsl::untar( "assets/zen.tar" ) );

  REQUIRE( uitsl::compare_files(
    "assets/beautiful-is-better-than-ugly-explicit-is-better-than-implicit-simple-is-better-than-complex-complex-is-better-than-complicated-flat-is-better-than-nested/although-practicality-beats-purity-errors-should-never-pass-silently-unless-explicitly-silenced-in-the-face-of-ambiguity-refuse-the-temptation-to-guess",
    "beautiful-is-better-than-ugly-explicit-is-better-than-implicit-simple-is-better-than-complex-complex-is-better-than-complicated-flat-is-better-than-nested/although-practicality-beats-purity-errors-should-never-pass-silently-unless-explicitly-silenced-in-the-face-of-ambiguity-refuse-the-temptation-to-guess"
  ) );

}
