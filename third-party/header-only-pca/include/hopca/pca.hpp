#pragma once

#include <stddef.h>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <functional>

#include "../../third-party/linalg/single-include/linalg.hpp"

#include "eigenmap.hpp"
#include "normalize.hpp"
#include "types.hpp"

namespace hopca {

class PCA {
    size_t n_components;
    double tolerance;
    size_t max_iter;

public:
    PCA(
        size_t _n_components,
        double _tolerance = 0.0001,
        size_t _max_iter = 1000
    )
    : n_components(_n_components)
    , tolerance(_tolerance)
    , max_iter(_max_iter)
    { ; }

private:
    hopca::Matrix normalize(hopca::Matrix m) const;
public:
    void setNComponents(const size_t n) { n_components = n; }
    void setTolerance(const double d) { tolerance = d; }
    void setMaxIter(const size_t n) { max_iter = n; }

    hopca::Matrix doPCANoNormalize(hopca::Matrix m) const;
    hopca::Matrix doPCA(hopca::Matrix m) const;
};

hopca::Matrix PCA::doPCANoNormalize(hopca::Matrix m) const {

    // find opposite of matrix
    std::transform(
        DATA(m),
        DATA(m) + m->n_col * m->n_row,
        DATA(m),
        [](const double a) {
            return -a;
        }
    );

    const hopca::Matrix cov = get_covariance(m);

    const hopca::Eigen eig = hola::eigen_solve(cov, tolerance, max_iter);

    // map of eigenvalues to eigenvectors
    hopca::Eigenmap eigenmap(eig);
    eigenmap.trim(n_components);

    return hola::matrix_multiply(m, eigenmap.get_matrix());
}

hopca::Matrix PCA::doPCA(hopca::Matrix m) const {
    return doPCANoNormalize( hopca::normalize(m) );
}

} // namespace hopca
