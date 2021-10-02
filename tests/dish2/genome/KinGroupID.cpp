#include <sstream>
#include <utility>

#include "Catch/single_include/catch2/catch.hpp"
#include "cereal/include/cereal/archives/binary.hpp"
#include "cereal/include/cereal/archives/json.hpp"
#include "conduit/include/uitsl/mpi/MpiGuard.hpp"

#include "dish2/genome/KinGroupID.hpp"
#include "dish2/spec/Spec.hpp"

using Spec = dish2::Spec_default;

const uitsl::MpiGuard guard;

TEST_CASE("Test Binary Serialization") {

  dish2::KinGroupID<Spec> original{ std::in_place };

  std::stringstream ss;

  {
    cereal::JSONOutputArchive oarchive(ss); // Create an output archive
    oarchive(original); // Write the data to the archive
  } // archive goes out of scope, ensuring all contents are flushed

  dish2::KinGroupID<Spec> dup;
  {
    cereal::JSONInputArchive iarchive(ss); // Create an input archive
    iarchive(dup);
  }

  REQUIRE( original == dup );

}

TEST_CASE("Test JSON Serialization") {

  dish2::KinGroupID<Spec> original{ std::in_place };

  std::stringstream ss;

  {
    cereal::JSONOutputArchive oarchive(ss); // Create an output archive
    oarchive(original); // Write the data to the archive
  } // archive goes out of scope, ensuring all contents are flushed

  dish2::KinGroupID<Spec> dup;
  {
    cereal::JSONInputArchive iarchive(ss); // Create an input archive
    iarchive(dup);
  }

  REQUIRE( original == dup );

}
