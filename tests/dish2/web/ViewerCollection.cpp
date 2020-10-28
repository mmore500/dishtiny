#define CATCH_CONFIG_MAIN

#include "Catch/single_include/catch2/catch.hpp"
#include "Empirical/source/web/NodeDomShim.h"

#include "dish2/web/ViewerCollection.hpp"

const emp::web::NodeDomShim shim;

TEST_CASE("Test ViewerCollection") {

  dish2::ViewerCollection{};

}
