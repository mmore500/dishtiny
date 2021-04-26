#pragma once
#ifndef UIT_SPOUTS_WRAPPERS_TRIVIALSPOUTWRAPPER_HPP_INCLUDE
#define UIT_SPOUTS_WRAPPERS_TRIVIALSPOUTWRAPPER_HPP_INCLUDE

#include "../../../uitsl/meta/IdentityWrapper.hpp"

namespace uit {

template<typename T_>
class TrivialSpoutWrapper {

public:
  using T = T_;

  template<typename Inlet>
  using inlet_wrapper_t = uitsl::IdentityWrapper<Inlet>;

  template<typename Outlet>
  using outlet_wrapper_t = uitsl::IdentityWrapper<Outlet>;

};



} // namespace uit

#endif // #ifndef UIT_SPOUTS_WRAPPERS_TRIVIALSPOUTWRAPPER_HPP_INCLUDE
