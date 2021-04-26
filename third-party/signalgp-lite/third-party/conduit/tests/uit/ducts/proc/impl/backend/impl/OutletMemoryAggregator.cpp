#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#include "Catch/single_include/catch2/catch.hpp"

#include "uit/ducts/proc/impl/backend/impl/AggregatorSpec.hpp"
#include "uit/ducts/proc/impl/backend/impl/OutletMemoryAggregator.hpp"
#include "uit/ducts/proc/put=dropping+get=stepping+type=cereal/inlet=RingIsend+outlet=Iprobe_c::IriOiDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

TEST_CASE("Test OutletMemoryAggregator") {

  // TODO flesh out stub test
  using AggregatorSpec = uit::AggregatorSpec<
    uit::ImplSpec<char>,
    uit::c::IriOiDuct
  >;
  uit::OutletMemoryAggregator<AggregatorSpec>{};

}
