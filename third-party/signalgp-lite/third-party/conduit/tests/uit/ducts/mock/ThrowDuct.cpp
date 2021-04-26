#include <mpi.h>

#define CATCH_CONFIG_DEFAULT_REPORTER "multiprocess"
#include "Catch/single_include/catch2/catch.hpp"

#include "uit/ducts/mock/ThrowDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

#define MSG_T int

TEST_CASE("Test ThrowDuct", "[nproc:1]") {

  // TODO flesh out stub tests
  uit::ThrowDuct<uit::ImplSpec<MSG_T>>{};

}
