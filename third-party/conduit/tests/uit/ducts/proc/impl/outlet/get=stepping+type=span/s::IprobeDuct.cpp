#include <memory>

#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#include "Catch/single_include/catch2/catch.hpp"

#include "Empirical/include/emp/base/vector.hpp"

#include "uit/ducts/proc/impl/outlet/get=stepping+type=span/s::IprobeDuct.hpp"
#include "uit/setup/ImplSpec.hpp"
#include "uit/setup/InterProcAddress.hpp"

TEST_CASE("Test s::IprobeDuct") {

  using ImplSpec = uit::MockSpec<emp::vector<char>>;
  using BackEnd = uit::s::IprobeDuct<ImplSpec>::BackEndImpl;

  // TODO flesh out stub test
  uit::InterProcAddress address;
  std::shared_ptr<BackEnd> backing{ std::make_shared<BackEnd>() };
  uit::s::IprobeDuct<ImplSpec>{ address, backing };

}
