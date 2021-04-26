#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "sgpl/hardware/Cpu.hpp"
#include "sgpl/spec/Spec.hpp"

using spec_t = sgpl::Spec<>;


TEST_CASE("Test Cpu") {

  // TODO flesh out stub test
  sgpl::Cpu<spec_t>{};

}
