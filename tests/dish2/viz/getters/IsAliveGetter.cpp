#define CATCH_CONFIG_MAIN

#include "Catch/single_include/catch2/catch.hpp"
#include "conduit/include/uitsl/mpi/MpiGuard.hpp"

#include "dish2/spec/Spec.hpp"
#include "dish2/viz/getters/IsAliveGetter.hpp"
#include "dish2/world/ProcWorld.hpp"
#include "dish2/world/ThreadWorld.hpp"

const uitsl::MpiGuard guard;

TEST_CASE("Test IsAliveGetter") {
  auto tw = dish2::ProcWorld<dish2::Spec>{}.MakeThreadWorld(0);
  dish2::IsAliveGetter<dish2::Spec>{ tw };
}
