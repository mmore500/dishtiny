#define CATCH_CONFIG_MAIN

#include "Catch/single_include/catch2/catch.hpp"

#include "dish2/record/drawings/GridDrawer.hpp"
#include "dish2/spec/Spec.hpp"
#include "dish2/viz/artists/DummyArtist.hpp"
#include "dish2/world/ProcWorld.hpp"
#include "dish2/world/ThreadWorld.hpp"

#include "conduit/include/uitsl/mpi/MpiGuard.hpp"

const uitsl::MpiGuard guard;

using grid_drawer_t = dish2::GridDrawer<
  dish2::Spec,
  dish2::DummyArtist
>;

TEST_CASE("Test copy ctor") {

  auto tw = dish2::ProcWorld<dish2::Spec>{}.MakeThreadWorld(0);

  grid_drawer_t drawer(tw, 0);

  grid_drawer_t drawer2 = drawer;

}

TEST_CASE("Test copy assignment operator") {

  auto tw = dish2::ProcWorld<dish2::Spec>{}.MakeThreadWorld(0);

  grid_drawer_t drawer(tw, 0);

  grid_drawer_t drawer2(tw, 0);

  drawer2 = drawer;

}
