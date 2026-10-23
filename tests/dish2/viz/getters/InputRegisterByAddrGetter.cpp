#include "Catch/single_include/catch2/catch.hpp"
#include "conduit/include/uitsl/mpi/MpiGuard.hpp"

#include "dish2/config/TemporaryThreadIdxOverride.hpp"
#include "dish2/peripheral/readable_state/ReadableState.hpp"
#include "dish2/spec/Spec.hpp"
#include "dish2/viz/getters/InputRegisterByAddrGetter.hpp"
#include "dish2/world/ProcWorld.hpp"
#include "dish2/world/ThreadWorld.hpp"

using Spec = dish2::Spec_default;

const dish2::TemporaryThreadIdxOverride override{ 0 };

const uitsl::MpiGuard guard;

using getter_t = dish2::InputRegisterByAddrGetter<Spec>;

TEST_CASE("Test InputRegisterByAddrGetter address mapping") {
  REQUIRE( getter_t::GetNumAddrs() == dish2::IntrospectiveState<Spec>::GetSize() );
  for (size_t addr{}; addr < getter_t::GetNumAddrs(); ++addr) {
    REQUIRE(
      dish2::ReadableState<Spec>::GetLeafTypeName( addr )
      == dish2::IntrospectiveState<Spec>::GetLeafTypeName( addr )
    );
  }
}

TEST_CASE("Test InputRegisterByAddrGetter") {
  auto tw = dish2::ProcWorld<Spec>{}.MakeThreadWorld();

  for (size_t addr{}; addr < getter_t::GetNumAddrs(); ++addr) {
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
