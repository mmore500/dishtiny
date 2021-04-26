#include <memory>

#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#include "Catch/single_include/catch2/catch.hpp"
#include "Empirical/include/emp/base/vector.hpp"

#include "uit/ducts/proc/impl/inlet/accumulating+type=spanfundamental/sf::AccumulateDuct.hpp"
#include "uit/setup/ImplSpec.hpp"
#include "uit/setup/InterProcAddress.hpp"

TEST_CASE("Test sf::AccumulateDuct") {

  using ImplSpec = uit::MockSpec<emp::vector<char>>;
  using BackEnd = uit::sf::AccumulateDuct<ImplSpec>::BackEndImpl;

  // TODO flesh out stub test
  uit::InterProcAddress address;
  std::shared_ptr<BackEnd> backing{ std::make_shared<BackEnd>( 42 ) };
  uit::sf::AccumulateDuct<ImplSpec>{ address, backing };

}
