#include "Catch/single_include/catch2/catch.hpp"
#include "conduit/include/uitsl/mpi/MpiGuard.hpp"

#include "dish2/config/TemporaryThreadIdxOverride.hpp"
#include "dish2/peripheral/readable_state/ReadableState.hpp"
#include "dish2/spec/Spec.hpp"
#include "dish2/viz/getters/ReadableStateByAddrGetter.hpp"
#include "dish2/world/ProcWorld.hpp"
#include "dish2/world/ThreadWorld.hpp"

using Spec = dish2::Spec_default;

const dish2::TemporaryThreadIdxOverride override{ 0 };

const uitsl::MpiGuard guard;

using getter_t = dish2::ReadableStateByAddrGetter<Spec>;

TEST_CASE("Test ReadableStateByAddrGetter address mapping") {
  for (size_t addr{}; addr < dish2::ReadableState<Spec>::GetSize(); ++addr) {
    REQUIRE(
      dish2::ReadableState<Spec>::GetLeafTypeName( addr )
      == dish2::ReadableState<Spec>::GetLeafTypeName( addr )
    );
  }
}

TEST_CASE("Test ReadableStateByAddrGetter") {
  auto tw = dish2::ProcWorld<Spec>{}.MakeThreadWorld();

  for (size_t addr{}; addr < dish2::ReadableState<Spec>::GetSize(); ++addr) {
    const getter_t getter{ tw, addr };
    REQUIRE( getter.GetNumCells() == tw.GetSize() );
    for (size_t cell{}; cell < getter.GetNumCells(); ++cell) {
      for (size_t dir{}; dir < getter.GetNumCardinals( cell ); ++dir) {
        const auto& readable_state
          = tw.GetCell( cell ).cardinals[ dir ].peripheral.readable_state;
        REQUIRE( getter.Get( cell, dir ) == readable_state.Read( addr ) );
      }
    }
  }

}
