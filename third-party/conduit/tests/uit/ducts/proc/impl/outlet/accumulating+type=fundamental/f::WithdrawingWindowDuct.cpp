#include <memory>

#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#include "Catch/single_include/catch2/catch.hpp"

#include "uit/ducts/proc/impl/outlet/accumulating+type=fundamental/f::WithdrawingWindowDuct.hpp"
#include "uit/setup/ImplSpec.hpp"
#include "uit/setup/InterProcAddress.hpp"

TEST_CASE("Test f::WithdrawingWindowDuct") {

  using ImplSpec = uit::MockSpec<char>;
  using BackEnd = uit::f::WithdrawingWindowDuct<ImplSpec>::BackEndImpl;

  // TODO flesh out stub test
  uit::InterProcAddress address;
  std::shared_ptr<BackEnd> backing{ std::make_shared<BackEnd>() };
  uit::f::WithdrawingWindowDuct<ImplSpec>{ address, backing };

}
