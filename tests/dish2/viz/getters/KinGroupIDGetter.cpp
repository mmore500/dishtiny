#define CATCH_CONFIG_MAIN

#include "Catch/single_include/catch2/catch.hpp"
#include "conduit/include/uitsl/mpi/MpiGuard.hpp"

#include "dish2/viz/getters/KinGroupIDGetter.hpp"
#include "dish2/world/ProcWorld.hpp"
#include "dish2/world/ThreadWorld.hpp"
#include "dish2/spec/Spec.hpp"

const uitsl::MpiGuard guard;

TEST_CASE("Test KinGroupIDGetter") {
  auto tw = dish2::ProcWorld<dish2::Spec>{}.MakeThreadWorld(0);
  dish2::KinGroupIDGetter<dish2::Spec>{ tw };
}
