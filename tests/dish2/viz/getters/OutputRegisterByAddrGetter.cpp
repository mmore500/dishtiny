#include "Catch/single_include/catch2/catch.hpp"
#include "conduit/include/uitsl/mpi/MpiGuard.hpp"

#include "dish2/config/TemporaryThreadIdxOverride.hpp"
#include "dish2/peripheral/readable_state/ReadableState.hpp"
#include "dish2/spec/Spec.hpp"
#include "dish2/viz/getters/OutputRegisterByAddrGetter.hpp"
#include "dish2/world/ProcWorld.hpp"
#include "dish2/world/ThreadWorld.hpp"

using Spec = dish2::Spec_default;

const dish2::TemporaryThreadIdxOverride override{ 0 };

const uitsl::MpiGuard guard;

using getter_t = dish2::OutputRegisterByAddrGetter<Spec>;

TEST_CASE("Test OutputRegisterByAddrGetter address mapping") {
  REQUIRE( getter_t::GetNumAddrs() == dish2::WritableState<Spec>::GetSize() );
  for (size_t addr{}; addr < getter_t::GetNumAddrs(); ++addr) {
    REQUIRE(
      dish2::ReadableState<Spec>::GetLeafTypeName(
        dish2::IntrospectiveState<Spec>::GetSize() + addr
      )
      == dish2::WritableState<Spec>::GetLeafTypeName( addr )
    );
  }
}

TEST_CASE("Test OutputRegisterByAddrGetter") {
  auto tw = dish2::ProcWorld<Spec>{}.MakeThreadWorld();

  for (size_t addr{}; addr < getter_t::GetNumAddrs(); ++addr) {
    const getter_t getter{ tw, addr };
    REQUIRE( getter.GetNumCells() == tw.GetSize() );
    for (size_t cell{}; cell < getter.GetNumCells(); ++cell) {
      for (size_t dir{}; dir < getter.GetNumCardinals( cell ); ++dir) {
        const auto& readable_state
          = tw.GetCell( cell ).cardinals[ dir ].peripheral.readable_state;
        REQUIRE( getter.Get( cell, dir ) == readable_state.Read(
          dish2::IntrospectiveState<Spec>::GetSize() + addr
        ) );
      }
    }
  }

  // values written by evolving programs are visible through the getter
  for (size_t addr{}; addr < getter_t::GetNumAddrs(); ++addr) {
    const getter_t getter{ tw, addr };
    auto& writable_state = tw.population[ 0 ].cardinals[ 0 ].peripheral
      .readable_state.template Get< dish2::WritableState<Spec> >();

    writable_state.Write( addr, 1.0f );
    REQUIRE( getter.Get( 0, 0 ) == 1.0 );

    writable_state.Write( addr, 0.0f );
    REQUIRE( getter.Get( 0, 0 ) == 0.0 );
  }

}
