#define CATCH_CONFIG_MAIN
#include "Catch/single_include/catch2/catch.hpp"

#include "Empirical/include/emp/math/Random.hpp"

#include "sgpl/library/OpLibraryCoupler.hpp"
#include "sgpl/library/prefab/CompleteOpLibrary.hpp"

struct ExampleOp {

  template<typename Spec>
  static void run(
    sgpl::Core<Spec>&,
    const sgpl::Instruction<Spec>&,
    const sgpl::Program<Spec>&,
    typename Spec::peripheral_t&
  ) {}

  static std::string name() { return "ExampleOp"; }

};


TEST_CASE("Test OpLibraryCoupler") {

  using library_t = sgpl::OpLibraryCoupler<
    sgpl::CompleteOpLibrary,
    ExampleOp
  >;

  REQUIRE(
    library_t::GetOpName( library_t::GetOpCode( "ExampleOp" ) ) == "ExampleOp"
  );

}
