#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#include "Catch/single_include/catch2/catch.hpp"
#include "conduit/include/uitsl/debug/MultiprocessReporter.hpp"
#include "conduit/include/uitsl/mpi/MpiGuard.hpp"

const uitsl::MpiGuard guard;

#include "dish2/config/TemporaryThreadIdxOverride.hpp"
#include "dish2/spec/Spec.hpp"
#include "dish2/world/ProcWorld.hpp"
#include "dish2/world/ThreadWorld.hpp"

using Spec = dish2::Spec_default;

const dish2::TemporaryThreadIdxOverride override{ 0 };

TEST_CASE("Test ThreadWorld") {

  auto tw = dish2::ProcWorld<Spec>{}.MakeThreadWorld();

  for (size_t i{}; i < std::centi::num; ++i) tw.Update();

}
