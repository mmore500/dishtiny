#pragma once
#ifndef UITSL_NONCE_SCOPEGUARD_HPP_INCLUDE
#define UITSL_NONCE_SCOPEGUARD_HPP_INCLUDE

#include <functional>

namespace uitsl {

class ScopeGuard {

  std::function<void()> on_destruct;

public:
  template <typename OnConstruct, typename OnDestruct=std::function<void()>>
  ScopeGuard(
    OnConstruct&& on_construct=[](){},
    OnDestruct&& on_destruct_=[](){}
  ) : on_destruct(on_destruct_) { on_construct(); }

  ~ScopeGuard() { on_destruct(); }

};

} // namespace uitsl

#endif // #ifndef UITSL_NONCE_SCOPEGUARD_HPP_INCLUDE
