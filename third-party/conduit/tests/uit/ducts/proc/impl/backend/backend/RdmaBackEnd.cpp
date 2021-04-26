#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#include "Catch/single_include/catch2/catch.hpp"

#include "uit/ducts/proc/impl/backend/RdmaBackEnd.hpp"
#include "uit/setup/ImplSpec.hpp"

TEST_CASE("Test RdmaBackEnd") {

  // TODO flesh out stub test
  uit::RdmaBackEnd<uit::ImplSpec<char>>{};

}
