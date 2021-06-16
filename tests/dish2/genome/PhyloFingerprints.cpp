#include <sstream>

#define CATCH_CONFIG_MAIN

#include "Catch/single_include/catch2/catch.hpp"
#include "cereal/include/cereal/archives/binary.hpp"
#include "cereal/include/cereal/archives/json.hpp"
#include "conduit/include/uitsl/mpi/MpiGuard.hpp"

#include "dish2/genome/PhyloFingerprints.hpp"
#include "dish2/spec/Spec.hpp"

using Spec = dish2::Spec_default;

const uitsl::MpiGuard guard;

// TEST_CASE("Test Binary Serialization") {
//
//   dish2::EventTags<Spec> original{};
//
//   std::stringstream ss;
//
//   {
//     cereal::JSONOutputArchive oarchive(ss); // Create an output archive
//     oarchive(original); // Write the data to the archive
//   } // archive goes out of scope, ensuring all contents are flushed
//
//   dish2::EventTags<Spec> dup;
//   {
//     cereal::JSONInputArchive iarchive(ss); // Create an input archive
//     iarchive(dup);
//   }
//
//   REQUIRE( original == dup );
//
// }
//
// TEST_CASE("Test JSON Serialization") {
//
//   dish2::EventTags<Spec> original{};
//
//   std::stringstream ss;
//
//   {
//     cereal::JSONOutputArchive oarchive(ss); // Create an output archive
//     oarchive(original); // Write the data to the archive
//   } // archive goes out of scope, ensuring all contents are flushed
//
//   dish2::EventTags<Spec> dup;
//   {
//     cereal::JSONInputArchive iarchive(ss); // Create an input archive
//     iarchive(dup);
//   }
//
//   REQUIRE( original == dup );
//
// }

TEST_CASE("get_lsb_index") {

  for (size_t gen{1}; gen < 100; ++gen) {
    // std::cout
    //   << gen << " "
    //   << dish2::PhyloFingerprints::get_lsb_index(gen) + 1
    //   << '\n';
  }

}

TEST_CASE("clear_msb") {

  for (size_t gen{}; gen < 100; ++gen) {
    // std::cout
    //   << gen << " "
    //   << dish2::PhyloFingerprints::clear_msb(gen)
    //   << '\n';
  }

}

TEST_CASE("calc_drop_idx") {

  for (size_t gen{}; gen < 100; ++gen) {
    // std::cout
    //   << gen << " "
    //   << dish2::PhyloFingerprints::calc_drop_idx(gen)
    //   << '\n';
  }

}

TEST_CASE("logrithmic growth") {
 // TODO
}

TEST_CASE("Deduction") {

  dish2::PhyloFingerprints parent;

  for (size_t gen{}; gen < 100; ++gen) parent.ElapseGeneration();

  dish2::PhyloFingerprints offspring = parent;

  for (size_t gen{}; gen < 100; ++gen) {
    std::cout
      << "actualy diverged " << gen << " generations ago,"
      << "deduced between "
      << parent.DivergedAtLeast( offspring )
      << " and "
      << parent.DivergedLessThan( offspring )
      << " generations\n";

      parent.ElapseGeneration();
      offspring.ElapseGeneration();
  }

}
