#include <sstream>

#define CATCH_CONFIG_MAIN

#include "Catch/single_include/catch2/catch.hpp"
#include "cereal/include/cereal/archives/json.hpp"

#include "dish2/genome/Genome.hpp"
#include "dish2/spec/Spec.hpp"

using Spec = dish2::Spec;

TEST_CASE("Test Genome") {
  dish2::Genome<Spec>{};
}

TEST_CASE("Test Serialization") {

  dish2::Genome<Spec> original{ 100 };

  std::stringstream ss; // any stream can be used

  {
    cereal::JSONOutputArchive oarchive(ss); // Create an output archive

    oarchive(original); // Write the data to the archive
  } // archive goes out of scope, ensuring all contents are flushed

  {
    cereal::JSONInputArchive iarchive(ss); // Create an input archive

    dish2::Genome<Spec> dup;
    iarchive(dup);
  }

  // REQUIRE( original == dup );

}
