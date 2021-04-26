#pragma once
#ifndef UITSL_UTILITY_NAMEDARRAYELEMENT_HPP_INCLUDE
#define UITSL_UTILITY_NAMEDARRAYELEMENT_HPP_INCLUDE

namespace uitsl {

// adapted from http://cplusplus.bordoon.com/namedArrayElements.html

template<size_t Index, class T>
struct NamedArrayElement {

  char _;  // needed to force minimal size of this object

  // allows: double d = object.Member;
  operator T&()
  {
  return ((T*)(this))[Index];
  }

  // allows: object.member = 1.0;
  T &operator=(T const &rhs) {
    T &me = ((T*)(this))[Index];

    me = rhs;

    return me;
  }

  // allows: double *p = &object.Member;
  T* operator&() {
    return &((T*)(this))[Index];
  }

};

} // namespace uitsl

#endif // #ifndef UITSL_UTILITY_NAMEDARRAYELEMENT_HPP_INCLUDE
