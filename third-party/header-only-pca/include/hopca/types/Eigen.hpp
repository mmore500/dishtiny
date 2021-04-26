#ifndef HOPCA_EIGEN
#define HOPCA_EIGEN

#include <memory>

#include "../../../third-party/linalg/single-include/linalg.hpp"

namespace hola {

  using eigen_t = struct eigen;
  using eigen_linalg_obj_t = struct linalg_obj;

} // namespace hola

namespace hopca {

class Eigen {

  using eigen_t = hola::eigen_t;
  using linalg_obj_t = hola::eigen_linalg_obj_t;
  std::shared_ptr<eigen_t> data;

public:

  Eigen() = default;

  Eigen( eigen_t* eigen )
  : data( eigen, hola::eigen_free )
  { ; }

  operator linalg_obj_t*() {
    return reinterpret_cast<linalg_obj_t*>( data.get() );
  }

  operator linalg_obj_t*() const {
    return reinterpret_cast<linalg_obj_t*>( data.get() );
  }

  eigen_t& operator*() { return *data; }

  const eigen_t& operator*() const { return *data; }

  eigen_t* operator->() { return data.get(); }

  const eigen_t* operator->() const { return data.get(); }

  bool operator==(std::nullptr_t) const { return data.get() == nullptr; }

  bool operator!=(std::nullptr_t) const { return data.get() != nullptr; }

};

} // namespace hopca

#endif // #define HOPCA_EIGEN
