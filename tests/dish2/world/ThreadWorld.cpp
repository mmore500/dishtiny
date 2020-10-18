#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#include "Catch/single_include/catch2/catch.hpp"
#include "conduit/include/uitsl/debug/MultiprocessReporter.hpp"
#include "conduit/include/uitsl/mpi/MpiGuard.hpp"

#include "dish2/spec/Spec.hpp"
#include "dish2/world/ProcWorld.hpp"
#include "dish2/world/ThreadWorld.hpp"

const uitsl::MpiGuard guard;

TEST_CASE("Test ThreadWorld") {

  auto tw = dish2::ProcWorld<dish2::Spec>{}.MakeThreadWorld(0);

  for (size_t i{}; i < std::centi::num; ++i) tw.Update();

}
