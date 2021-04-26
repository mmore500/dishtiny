#ifndef EIGENMAP_PCA
#define EIGENMAP_PCA

#include <assert.h>
#include <map>

#include "types.hpp"

#include "../../third-party/linalg/single-include/linalg.hpp"

namespace hopca {

class Eigenmap {
    std::map<
        double,
        hopca::Vector,
        std::greater<double>
    > map;
    const size_t rows;
    size_t cols;

public:
    Eigenmap(hopca::Eigen eigen)
    : rows(eigen->eigenvectors->n_row)
    , cols(eigen->eigenvectors->n_col)
    {
        for (int i = 0; i < eigen->n; ++i) {
            const double val = VECTOR_IDX_INTO(eigen->eigenvalues, i);
            const hopca::Vector vect
              = hola::matrix_column_copy(eigen->eigenvectors, i);
            assert(vect != nullptr);
            map[val] = vect;
        }
    }

    hopca::Vector operator[](const size_t i) { return map[i]; }

    /// Trims the map, turning it into a map with n columns.
    void trim(const size_t n) {
        map.erase(
            std::next(
                map.begin(),
                std::min(n, map.size())
            ),
            map.end()
        );
        cols = n;
    }

    hopca::Matrix get_matrix() const {
        hopca::Matrix temp = hola::matrix_new(rows, cols);
        assert(temp != nullptr);

        size_t i = 0;
        for (auto [_k, v] : map) {
            hola::matrix_copy_vector_into_column(temp, v, i);
            ++i;
        }
        return temp;
    }
};

} // namespace hopca

#endif
