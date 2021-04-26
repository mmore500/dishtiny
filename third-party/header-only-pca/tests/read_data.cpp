#include <iostream>
#include <sstream>
#include <vector>

#include "Catch/single_include/catch2/catch.hpp"

#include "hopca/read_data.hpp"
#include "hopca/types.hpp"

std::stringstream make_ss(const std::vector<double>& matrix, const size_t m, const size_t n) {
    std::stringstream ss;

    for (size_t i = 0; i < m; ++i) {
        for (size_t j = 0; j < n; ++j) {
            ss << matrix[i*n+ j] << " ";
        }
        ss << "\n";
    }

    return ss;
}

TEST_CASE("Make sure read_data matches up with known matrix") {
    std::stringstream ss(
        "3 1 0\n\
        3 0.5 7\n\
        5 8 10"
    );

    const hopca::Matrix res = hopca::read_data(3, 3, ss);

    std::vector<double> m{3, 1, 0, 3, 0.5, 7, 5, 8, 10};

    const hopca::Matrix known = hola::matrix_from_array(m.data(), 3, 3);

    REQUIRE(hola::matrix_equal(res, known, 0.000001));
}

TEST_CASE("Check that non-square matrices work") {
    std::vector<double> m{1, 0.3, 2, 54, 10, 0, -4, 2, 9, 12, 4, 5};

    std::stringstream ss = make_ss(m, 4, 3);

    const hopca::Matrix known = hola::matrix_from_array(m.data(), 4, 3);

    const hopca::Matrix res = hopca::read_data(4, 3, ss);

    REQUIRE(hola::matrix_equal(res, known, 0.000001));
}
