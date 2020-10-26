#define CATCH_CONFIG_MAIN

#include "Catch/single_include/catch2/catch.hpp"

#include "dish2/spec/Spec.hpp"
#include "dish2/web/fill_colormaps/PhylogeneticRootColorMap.hpp"
#include "dish2/web/getters/PhylogeneticRootGetter.hpp"
#include "dish2/world/ProcWorld.hpp"
#include "dish2/world/ThreadWorld.hpp"

#include "conduit/include/uitsl/mpi/MpiGuard.hpp"

const uitsl::MpiGuard guard;

TEST_CASE("Test PhylogeneticRootColorMap") {
  auto tw = dish2::ProcWorld<dish2::Spec>{}.MakeThreadWorld(0);
  dish2::PhylogeneticRootGetter<dish2::Spec> getter{ tw };
  dish2::PhylogeneticRootColorMap{ getter };
}
