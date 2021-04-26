#pragma once
#ifndef UITSL_MATH_MAPPING_UTILS_HPP_INCLUDE
#define UITSL_MATH_MAPPING_UTILS_HPP_INCLUDE

#include <cmath>

#include "../../../third-party/Empirical/include/emp/base/vector.hpp"

namespace uitsl {

using Point = emp::vector<size_t>;
using Dims = emp::vector<size_t>;

/**
 * This function maps a point in a finite N-dimensional space to 1-dimensional space
 * The cardinality of the dimensions of the space need not be equal
 * @param p point to map
 * @param dims dimensions of space
 * @return 1-dimensional mapping
*/
inline size_t linear_encode(const Point& p, const Dims& dims) {
    size_t mx = 1;
    size_t encoded = 0;
    for (size_t i = 0; i < dims.size(); ++i) {
        encoded += p[i] * mx;
        mx *= dims[i];
    }
    return encoded;
}

/**
 * This function maps a point in a 1-dimensional space to a finite N-dimensional space
 * The cardinality of the dimensions of the space need not be equal
 * @param p point to map
 * @param dims dimensions of space
 * @return 1-dimensional mapping
*/
inline Point linear_decode(size_t r, const Dims& dims) {
    emp::vector<size_t> decoded;
    for (size_t i = 0; i < dims.size(); ++i) {
        decoded.push_back(r % dims[i]);
        r /= dims[i];
    }
    return decoded;
}

// TODO: add a Hilbert curve variant

} // namespace uitsl

#endif // #ifndef UITSL_MATH_MAPPING_UTILS_HPP_INCLUDE
