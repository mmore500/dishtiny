#ifndef HOPCA_VECTOR
#define HOPCA_VECTOR

#include <cstddef>
#include <memory>

#include "../../../third-party/linalg/single-include/linalg.hpp"

namespace hola {

  using vector_t = struct vector;
  using vector_linalg_obj_t = struct linalg_obj;

} // namespace hola

namespace hopca {

class Vector {

  using vector_t = hola::vector_t;
  using linalg_obj_t = hola::vector_linalg_obj_t;

  std::shared_ptr<vector_t> data;

public:

  Vector() = default;

  Vector( vector_t* vector )
  : data( vector, hola::vector_free )
  { ; }

  operator vector_t*() { return data.get(); }

  operator vector_t*() const { return data.get(); }

  operator linalg_obj_t*() {
    return reinterpret_cast<linalg_obj_t*>( data.get() );
  }

  operator linalg_obj_t*() const {
    return reinterpret_cast<linalg_obj_t*>( data.get() );
  }

  vector_t& operator*() { return *data; }

  const vector_t& operator*() const { return *data; }

  vector_t* operator->() { return data.get(); }

  const vector_t* operator->() const { return data.get(); }

  bool operator==(std::nullptr_t) const { return data.get() == nullptr; }

  bool operator!=(std::nullptr_t) const { return data.get() != nullptr; }

};

} // namespace hoppca

#endif // #define HOPCA_VECTOR
