#include <memory>

#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#include "Catch/single_include/catch2/catch.hpp"

#include "Empirical/include/emp/base/vector.hpp"

#include "uit/ducts/proc/impl/inlet/put=dropping+type=span/s::RingIrsendDuct.hpp"
#include "uit/setup/ImplSpec.hpp"
#include "uit/setup/InterProcAddress.hpp"

TEST_CASE("Test s::RingIrsendDuct") {

  using ImplSpec = uit::MockSpec<emp::vector<char>>;
  using BackEnd = uit::s::RingIrsendDuct<ImplSpec>::BackEndImpl;

  // TODO flesh out stub test
  uit::InterProcAddress address;
  std::shared_ptr<BackEnd> backing{ std::make_shared<BackEnd>() };
  uit::s::RingIrsendDuct<ImplSpec>{ address, backing };

}
