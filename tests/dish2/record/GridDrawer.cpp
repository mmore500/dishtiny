#include "Catch/single_include/catch2/catch.hpp"
#include "conduit/include/uitsl/mpi/MpiGuard.hpp"

#include "dish2/config/thread_idx.hpp"
#include "dish2/record/drawings/GridDrawer.hpp"
#include "dish2/spec/Spec.hpp"
#include "dish2/viz/artists/DummyArtist.hpp"
#include "dish2/world/ProcWorld.hpp"
#include "dish2/world/ThreadWorld.hpp"

const uitsl::MpiGuard guard;

using Spec = dish2::Spec_default;

using grid_drawer_t = dish2::GridDrawer<
  Spec,
  dish2::DummyArtist
>;

TEST_CASE("Test copy ctor") {

  dish2::thread_idx = 0;

  auto tw = dish2::ProcWorld<Spec>{}.MakeThreadWorld();

  grid_drawer_t drawer(tw, 0);

  grid_drawer_t drawer2 = drawer;

}

TEST_CASE("Test copy assignment operator") {

  dish2::thread_idx = 0;

  auto tw = dish2::ProcWorld<Spec>{}.MakeThreadWorld();

  grid_drawer_t drawer(tw, 0);

  grid_drawer_t drawer2(tw, 0);

  drawer2 = drawer;

}
