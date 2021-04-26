#pragma once
#ifndef UITSL_META_IDENTITYWRAPPER_HPP_INCLUDE
#define UITSL_META_IDENTITYWRAPPER_HPP_INCLUDE

namespace uitsl {

template<typename T>
class IdentityWrapper : public T {

  // inherit parent's constructors
  // adapted from https://stackoverflow.com/a/434784
  using T::T;

};

} // namespace uitsl

#endif // #ifndef UITSL_META_IDENTITYWRAPPER_HPP_INCLUDE
