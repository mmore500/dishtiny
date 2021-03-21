#define CATCH_CONFIG_MAIN

#include "Catch/single_include/catch2/catch.hpp"
#include "conduit/include/uitsl/mpi/MpiGuard.hpp"

#include "dish2/spec/Spec.hpp"
#include "dish2/viz/getters/PhylogeneticRootGetter.hpp"
#include "dish2/world/ProcWorld.hpp"
#include "dish2/world/ThreadWorld.hpp"

using Spec = dish2::Spec_default;

const uitsl::MpiGuard guard;

TEST_CASE("Test PhylogeneticRootGetter") {
  auto tw = dish2::ProcWorld<Spec>{}.MakeThreadWorld(0);
  dish2::PhylogeneticRootGetter<Spec>{ tw };
}
