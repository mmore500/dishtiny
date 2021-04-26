#pragma once

#include <algorithm>
#include <optional>

#include "../../third-party/linalg/single-include/linalg.hpp"

#include "types.hpp"

namespace hopca {

inline hopca::Matrix get_covariance(const hopca::Matrix m) {
    hopca::Matrix ret = hola::matrix_multiply_MtN(m, m);

    std::transform(
        DATA(ret),
        DATA(ret) + ret->n_col * ret->n_row,
        DATA(ret),
        [n = ret->n_col](const double a) {
            return a / n;
        }
    );

    return ret;
}

inline double get_mean(const hopca::Vector v) {
    const size_t len = v->length;
    double sum = 0;

    for (size_t i = 0; i < len; ++i) {
        sum += VECTOR_IDX_INTO(v, i);
    }

    return sum / len;
}

inline double get_max(const hopca::Vector v) {
    return *std::max_element(
      DATA(v),
      DATA(v) + v->length
    );
}

inline double get_min(const hopca::Vector v) {
  return *std::min_element(
    DATA(v),
    DATA(v) + v->length
  );
}

inline bool is_homogenous(const hopca::Vector v) {
  return hopca::get_min( v ) == hopca::get_max( v );
}

inline size_t count_homogenous_rows(const hopca::Matrix m) {
  const size_t rows = m->n_row;

  size_t res{};

  for (size_t row = 0; row < rows; ++row) {

    hopca::Vector vec = hola::matrix_row_copy(m, row);

    res += hopca::is_homogenous( vec );

  }

  return res;
}

inline size_t count_homogenous_columns(const hopca::Matrix m) {
  const size_t cols = m->n_col;

  size_t res{};

  for (size_t col = 0; col < cols; ++col) {

    hopca::Vector vec = hola::matrix_column_copy(m, col);

    res += hopca::is_homogenous( vec );

  }

  return res;
}

inline std::optional<hopca::Matrix>
drop_homogenous_rows( const hopca::Matrix m ) {

  const size_t rows_before = m->n_row;
  const size_t cols = m->n_col;
  const size_t rows_after = rows_before - hopca::count_homogenous_rows( m );

  if ( rows_after == 0 ) return std::nullopt;

  hopca::Matrix res = hola::matrix_new(rows_after, cols);
  assert(res != nullptr);

  size_t res_row{};
  for (size_t row = 0; row < rows_before; ++row) {
    hopca::Vector vec = hola::matrix_row_copy(m, row);
    if ( !hopca::is_homogenous( vec ) ) {
      hola::matrix_copy_vector_into_row(res, vec, res_row++);
    }
  }

  return res;

}

inline std::optional<hopca::Matrix>
drop_homogenous_columns( const hopca::Matrix m ) {

  const size_t rows = m->n_row;
  const size_t cols_before = m->n_col;
  const size_t cols_after = cols_before - hopca::count_homogenous_columns( m );

  if ( cols_after == 0 ) return std::nullopt;

  hopca::Matrix res = hola::matrix_new(rows, cols_after);
  assert(res != nullptr);

  size_t res_col{};
  for (size_t col = 0; col < cols_before; ++col) {
    hopca::Vector vec = hola::matrix_column_copy(m, col);
    if ( !hopca::is_homogenous( vec ) ) {
      hola::matrix_copy_vector_into_column(res, vec, res_col++);
    }
  }

  return res;

}

inline hopca::Matrix normalize(hopca::Matrix m) {
    const size_t rows = m->n_row;
    const size_t cols = m->n_col;

    hopca::Matrix temp = hola::matrix_new(rows, cols);
    assert(temp != nullptr);

    for (size_t col = 0; col < cols; ++col) {
        hopca::Vector vec = hola::matrix_column_copy(m, col);

        const hopca::Vector mean_vector
          = hola::vector_constant(rows, hopca::get_mean(vec));
        hola::vector_subtract_into(vec, vec, mean_vector);

        const double norm_squared = hola::vector_dot_product(vec, vec);
        const double invstdev
          = norm_squared ? std::sqrt(rows / norm_squared) : 0.0;

        hola::vector_scalar_multiply_into(vec, vec, invstdev);

        hola::matrix_copy_vector_into_column(temp, vec, col);
    }
    return temp;
}

inline hopca::Matrix unit_normalize(hopca::Matrix m) {
    const size_t rows = m->n_row;
    const size_t cols = m->n_col;

    hopca::Matrix temp = hola::matrix_new(rows, cols);
    assert(temp != nullptr);

    for (size_t col = 0; col < cols; ++col) {
        hopca::Vector vec = hola::matrix_column_copy(m, col);

        const hopca::Vector min_vector
          = hola::vector_constant(rows, hopca::get_min(vec));
        hola::vector_subtract_into(vec, vec, min_vector);

        const double diff = hopca::get_max(vec) - hopca::get_min(vec);
        const double invdiff = diff ? std::sqrt(1 / diff) : 0.0;

        hola::vector_scalar_multiply_into(vec, vec, invdiff);

        hola::matrix_copy_vector_into_column(temp, vec, col);
    }
    return temp;
}

} // namespace hopca
