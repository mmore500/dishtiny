#include "Catch/single_include/catch2/catch.hpp"
#include "Empirical/include/emp/web/NodeDomShim.hpp"

#include "dish2/web/ControlPanel.hpp"

const emp::web::NodeDomShim shim{{ "emp_button_dash" }};

TEST_CASE("Test ControlPanel") {

  new dish2::ControlPanel( [](const bool, const size_t){ return 0ul;} );

}
