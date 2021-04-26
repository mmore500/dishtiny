#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "sgpl/hardware/JumpTable.hpp"
#include "sgpl/spec/Spec.hpp"

using spec_t = sgpl::Spec<>;

TEST_CASE("Test JumpTable") {

  // TODO flesh out stub test
  sgpl::JumpTable<spec_t, typename spec_t::global_matching_t>{};

}
