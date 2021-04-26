#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#include "Catch/single_include/catch2/catch.hpp"

#include "uit/ducts/proc/impl/backend/MockBackEnd.hpp"
#include "uit/setup/ImplSpec.hpp"

TEST_CASE("Test MockBackEnd") {

  // TODO flesh out stub test
  uit::MockBackEnd<uit::ImplSpec<char>>{};

}
