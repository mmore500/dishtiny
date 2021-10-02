#include "Catch/single_include/catch2/catch.hpp"
#include "Empirical/include/emp/web/Document.hpp"
#include "Empirical/include/emp/web/NodeDomShim.hpp"

#include "dish2/web/Animator.hpp"

const emp::web::NodeDomShim shim;

TEST_CASE("Test Animator") {

  dish2::Animator animator{ [](const bool){} };

  animator.ToggleRender();

  animator.DoFrame();

}
