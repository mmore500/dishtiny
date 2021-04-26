#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/debug/list_cwd.hpp"

TEST_CASE("list_cwd", "[nproc:1]") {

  uitsl::list_cwd();

}
