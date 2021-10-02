#include <sstream>
#include <utility>

#include "Catch/single_include/catch2/catch.hpp"
#include "cereal/include/cereal/archives/json.hpp"
#include "conduit/include/uitsl/mpi/MpiGuard.hpp"

#include "dish2/genome/Genome.hpp"
#include "dish2/spec/Spec.hpp"

using Spec = dish2::Spec_default;

const uitsl::MpiGuard guard;

TEST_CASE("Test Serialization") {

  dish2::Genome<Spec> original{ std::in_place };

  std::stringstream ss; // any stream can be used

  {
    cereal::JSONOutputArchive oarchive(ss); // Create an output archive
    oarchive(original); // Write the data to the archive
  } // archive goes out of scope, ensuring all contents are flushed

  dish2::Genome<Spec> dup;

  {
    cereal::JSONInputArchive iarchive(ss); // Create an input archive
    iarchive(dup);
  }

  REQUIRE( original == dup );

}
