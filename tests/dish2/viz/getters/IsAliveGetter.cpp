#define CATCH_CONFIG_MAIN

#include "Catch/single_include/catch2/catch.hpp"
#include "conduit/include/uitsl/mpi/MpiGuard.hpp"

#include "dish2/config/TemporaryThreadIdxOverride.hpp"
#include "dish2/spec/Spec.hpp"
#include "dish2/viz/getters/IsAliveGetter.hpp"
#include "dish2/world/ProcWorld.hpp"
#include "dish2/world/ThreadWorld.hpp"

using Spec = dish2::Spec_default;

const dish2::TemporaryThreadIdxOverride override{ 0 };

const uitsl::MpiGuard guard;

TEST_CASE("Test IsAliveGetter") {
  auto tw = dish2::ProcWorld<Spec>{}.MakeThreadWorld();
  dish2::IsAliveGetter<Spec>{ tw };
}
