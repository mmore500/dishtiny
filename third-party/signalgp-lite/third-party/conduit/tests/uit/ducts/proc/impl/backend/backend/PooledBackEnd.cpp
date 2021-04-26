#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#include "Catch/single_include/catch2/catch.hpp"
#include "Empirical/include/emp/base/vector.hpp"

#include "uit/ducts/proc/impl/backend/PooledBackEnd.hpp"
#include "uit/ducts/proc/put=dropping+get=skipping+type=span/inlet=RingIsend+outlet=BlockIrecv_s::IriObiDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

template<typename ImplSpec>
using PoolDuct = uit::s::IriObiDuct<ImplSpec>;

TEST_CASE("Test PooledBackEnd") {

  // TODO flesh out stub test
  using Spec = uit::ImplSpec<char>;


  uit::PooledBackEnd< Spec, PoolDuct >{};

}
