#include <memory>

#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#include "Catch/single_include/catch2/catch.hpp"

#include "uit/ducts/proc/impl/inlet/accumulating+type=fundamental/f::RaccumulateDuct.hpp"
#include "uit/setup/ImplSpec.hpp"
#include "uit/setup/InterProcAddress.hpp"

TEST_CASE("Test f::RaccumulateDuct") {

  using ImplSpec = uit::MockSpec<char>;
  using BackEnd = uit::f::RaccumulateDuct<ImplSpec>::BackEndImpl;

  // TODO flesh out stub test
  uit::InterProcAddress address;
  std::shared_ptr<BackEnd> backing{ std::make_shared<BackEnd>() };
  uit::f::RaccumulateDuct<ImplSpec>{ address, backing };

}
