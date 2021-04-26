#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#include "Catch/single_include/catch2/catch.hpp"
#include "Empirical/include/emp/base/vector.hpp"

#include "uit/ducts/proc/accumulating+type=spanfundamental/inlet=Accumulate+outlet=WithdrawingWindow_sf::IaOwwDuct.hpp"
#include "uit/ducts/proc/impl/backend/AccumulatingPooledBackEnd.hpp"
#include "uit/setup/ImplSpec.hpp"

template<typename ImplSpec>
using PoolDuct = uit::sf::IaOwwDuct<ImplSpec>;

TEST_CASE("Test AccumulatingPooledBackEnd") {

  // TODO flesh out stub test
  using Spec = uit::ImplSpec<char>;


  uit::AccumulatingPooledBackEnd< Spec, PoolDuct >{};

}
