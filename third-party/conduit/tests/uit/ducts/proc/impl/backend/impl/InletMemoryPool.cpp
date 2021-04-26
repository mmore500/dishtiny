#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#include "Catch/single_include/catch2/catch.hpp"

#include "uit/ducts/proc/impl/backend/impl/InletMemoryPool.hpp"
#include "uit/ducts/proc/impl/backend/impl/PoolSpec.hpp"
#include "uit/ducts/proc/put=dropping+get=skipping+type=span/inlet=RingIsend+outlet=BlockIrecv_s::IriObiDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

TEST_CASE("Test InletMemoryPool") {

  // TODO flesh out stub test
  using PoolSpec = uit::PoolSpec<
    uit::ImplSpec<char>,
    uit::s::IriObiDuct
  >;
  uit::InletMemoryPool<PoolSpec>{};

}
