#pragma once
#ifndef UITSL_META_TYPENAME_HPP_INCLUDE
#define UITSL_META_TYPENAME_HPP_INCLUDE

#include <typeinfo>

namespace uitsl {

  // adapted from https://stackoverflow.com/a/4485051

  // default implementation
  template <typename T>
  struct TypeName {
    static const char* Get() { return typeid(T).name(); }
  };

  #define UITSL_ENABLE_TYPENAME(A) \
  template<> \
  struct TypeName<A> { \
    static const char* Get() { return #A; } \
  };

  #define UITSL_ENABLE_TYPENAME_TEMPLATE(A) \
  template <typename... Args> \
  struct TypeName< A<Args...> > { \
    static const char* Get() { return #A; } \
  };

} // namespace uitsl

#endif // #ifndef UITSL_META_TYPENAME_HPP_INCLUDE
