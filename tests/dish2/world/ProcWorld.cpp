#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#include "Catch/single_include/catch2/catch.hpp"
#include "conduit/include/uitsl/debug/MultiprocessReporter.hpp"
#include "conduit/include/uitsl/mpi/MpiGuard.hpp"

#include "dish2/spec/Spec.hpp"
#include "dish2/world/ProcWorld.hpp"

using Spec = dish2::Spec_default;

const uitsl::MpiGuard guard;

TEST_CASE("Test ProcWorld") {
  dish2::ProcWorld<Spec>{};
}
