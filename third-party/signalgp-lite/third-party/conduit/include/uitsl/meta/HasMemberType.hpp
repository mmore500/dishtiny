#pragma once
#ifndef UITSL_META_HASMEMBERTYPE_HPP_INCLUDE
#define UITSL_META_HASMEMBERTYPE_HPP_INCLUDE

#include <type_traits>

// adapted from https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Member_Detector
#define UITSL_GENERATE_HAS_MEMBER_TYPE(Type) \
  \
template<typename  T> \
class HasMemberType_##Type \
{ \
private: \
  using Yes = char[2]; \
  using  No = char[1]; \
  \
  struct Fallback { struct Type { }; }; \
  struct Derived : T, Fallback { }; \
  \
  template < class U > \
  static No& test ( typename U::Type* ); \
  template < typename U > \
  static Yes& test ( U* ); \
  \
public: \
  static constexpr bool value = sizeof(test<Derived>(nullptr)) == sizeof(Yes);\
};

#endif // #ifndef UITSL_META_HASMEMBERTYPE_HPP_INCLUDE
