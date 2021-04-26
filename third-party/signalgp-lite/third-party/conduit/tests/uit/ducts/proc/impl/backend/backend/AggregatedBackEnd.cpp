#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#include "Catch/single_include/catch2/catch.hpp"
#include "Empirical/include/emp/base/vector.hpp"

#include "uit/ducts/proc/impl/backend/AggregatedBackEnd.hpp"
#include "uit/ducts/proc/put=dropping+get=stepping+type=cereal/inlet=RingIsend+outlet=Iprobe_c::IriOiDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

template<typename ImplSpec>
using AggregatorDuct = uit::c::IriOiDuct<ImplSpec>;

TEST_CASE("Test AggregatedBackEnd") {

  // TODO flesh out stub test
  using Spec = uit::ImplSpec<char>;


  uit::AggregatedBackEnd< Spec, AggregatorDuct >{};

}
