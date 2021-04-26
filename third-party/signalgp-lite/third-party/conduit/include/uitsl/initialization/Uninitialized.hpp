#pragma once
#ifndef UITSL_INITIALIZATION_UNINITIALIZED_HPP_INCLUDE
#define UITSL_INITIALIZATION_UNINITIALIZED_HPP_INCLUDE

#include <utility>

namespace uitsl {

template<typename T>
class Uninitialized {

  T val;

public:

  Uninitialized() { ; }
  explicit Uninitialized(const T& val_) : val(val_) { ; }

  operator const T&() const { return val; }
  operator T&() { return val; }

  T* operator&() { return &val; }

  // TODO cpp20 spaceship operator
  // https://devblogs.microsoft.com/cppblog/simplify-your-code-with-rocket-science-c20s-spaceship-operator/
  bool operator==(const T& rhs) const { return val == rhs; }

  bool operator!=(const T& rhs) const { return val != rhs; }

  T& operator=(const T& other) { return val = other; }

  T& operator=(T&& other) noexcept { return val = std::move(other); }

};

} // namespace uitsl

#endif // #ifndef UITSL_INITIALIZATION_UNINITIALIZED_HPP_INCLUDE
