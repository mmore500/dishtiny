#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#include "Catch/single_include/catch2/catch.hpp"
#include "Empirical/include/emp/base/vector.hpp"

#include "uit/ducts/proc/impl/backend/RuntimeSizeBackEnd.hpp"
#include "uit/setup/ImplSpec.hpp"

TEST_CASE("Test RuntimeSizeBackEnd") {

  // TODO flesh out stub test
  uit::RuntimeSizeBackEnd<uit::ImplSpec<emp::vector<char>>>{ 42 };

}
