#ifndef HOPCA_MATRIX
#define HOPCA_MATRIX

#include <memory>

#include "../../../third-party/linalg/single-include/linalg.hpp"

namespace hola {

  using matrix_t = struct matrix;
  using matrix_linalg_obj_t = struct linalg_obj;

} // namespace hola

namespace hopca {

class Matrix {

  using matrix_t = hola::matrix_t;
  using linalg_obj_t = hola::matrix_linalg_obj_t;

  std::shared_ptr<matrix_t> data;

public:

  Matrix() = default;

  Matrix( matrix_t* matrix )
  : data( matrix, hola::matrix_free )
  { ; }

  operator matrix_t*() { return data.get(); }

  operator matrix_t*() const { return data.get(); }

  operator linalg_obj_t*() {
    return reinterpret_cast<linalg_obj_t*>( data.get() );
  }

  operator linalg_obj_t*() const {
    return reinterpret_cast<linalg_obj_t*>( data.get() );
  }

  matrix_t& operator*() { return *data; }

  const matrix_t& operator*() const { return *data; }

  matrix_t* operator->() { return data.get(); }

  const matrix_t* operator->() const { return data.get(); }

  bool operator==(std::nullptr_t) const { return data.get() == nullptr; }

  bool operator!=(std::nullptr_t) const { return data.get() != nullptr; }

};

} // namespace hopca

#endif // #define HOPCA_MATRIX
