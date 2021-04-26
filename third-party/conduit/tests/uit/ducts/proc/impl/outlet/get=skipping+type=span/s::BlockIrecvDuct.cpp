#include <memory>

#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#include "Catch/single_include/catch2/catch.hpp"
#include "Empirical/include/emp/base/vector.hpp"

#include "uit/ducts/proc/impl/outlet/get=skipping+type=span/s::BlockIrecvDuct.hpp"
#include "uit/setup/ImplSpec.hpp"
#include "uit/setup/InterProcAddress.hpp"

TEST_CASE("Test s::BlockIrecvDuct") {

  using ImplSpec = uit::MockSpec<emp::vector<char>>;
  using BackEnd = uit::s::BlockIrecvDuct<ImplSpec>::BackEndImpl;

  // TODO flesh out stub test
  uit::InterProcAddress address;
  std::shared_ptr<BackEnd> backing{ std::make_shared<BackEnd>(42) };
  uit::s::BlockIrecvDuct<ImplSpec>{ address, backing };

}
