#include <memory>

#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#include "Catch/single_include/catch2/catch.hpp"

#include "uit/ducts/proc/impl/inlet/put=growing+type=trivial/t::DequeIsendDuct.hpp"
#include "uit/setup/ImplSpec.hpp"
#include "uit/setup/InterProcAddress.hpp"

TEST_CASE("Test t::DequeIsendDuct") {

  using ImplSpec = uit::MockSpec<char>;
  using BackEnd = uit::t::DequeIsendDuct<ImplSpec>::BackEndImpl;

  // TODO flesh out stub test
  uit::InterProcAddress address;
  std::shared_ptr<BackEnd> backing{ std::make_shared<BackEnd>() };
  uit::t::DequeIsendDuct<ImplSpec>{ address, backing };

}
