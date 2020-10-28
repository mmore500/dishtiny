#define CATCH_CONFIG_MAIN

#include "Catch/single_include/catch2/catch.hpp"
#include "Empirical/source/web/Document.h"
#include "Empirical/source/web/NodeDomShim.h"

#include "dish2/web/Animator.hpp"

const emp::web::NodeDomShim shim;

TEST_CASE("Test Animator") {

  dish2::Animator animator{ [](const bool){} };

  animator.ToggleRender();

  animator.DoFrame();

}
