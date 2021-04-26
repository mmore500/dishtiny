#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#include "Catch/single_include/catch2/catch.hpp"

#include "uit/ducts/proc/accumulating+type=spanfundamental/inlet=Accumulate+outlet=WithdrawingWindow_sf::IaOwwDuct.hpp"
#include "uit/ducts/proc/impl/backend/impl/InletMemoryAccumulatingPool.hpp"
#include "uit/ducts/proc/impl/backend/impl/PoolSpec.hpp"
#include "uit/setup/ImplSpec.hpp"

TEST_CASE("Test InletMemoryAccumulatingPool") {

  // TODO flesh out stub test
  using PoolSpec = uit::PoolSpec<
    uit::ImplSpec<char>,
    uit::sf::IaOwwDuct
  >;
  uit::InletMemoryAccumulatingPool<PoolSpec>{};

}
