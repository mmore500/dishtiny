#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#include "Catch/single_include/catch2/catch.hpp"
#include "Empirical/include/emp/base/vector.hpp"

#include "uit/ducts/proc/impl/backend/impl/AggregatorSpec.hpp"
#include "uit/ducts/proc/put=dropping+get=stepping+type=cereal/inlet=RingIsend+outlet=Iprobe_c::IriOiDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

TEST_CASE("Test AggregatorSpec") {

  // TODO flesh out stub test
  uit::AggregatorSpec<uit::ImplSpec<char>, uit::c::IriOiDuct>{};

}
