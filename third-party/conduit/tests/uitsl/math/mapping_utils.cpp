#include "Catch/single_include/catch2/catch.hpp"

#include "uitsl/math/mapping_utils.hpp"

TEST_CASE("Test linear encoder/decoder", "[nproc:1]") {
    const size_t a = 9;
    const size_t b = 7;
    const size_t c = 2;
    const uitsl::Dims dims{a, b, c};

    for (size_t i = 0; i < a * b * c ; ++i) {
        REQUIRE(uitsl::linear_encode(uitsl::linear_decode(i, dims), dims) == i);
    }
}
