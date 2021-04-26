#pragma once
#ifndef UITSL_META_HASMEMBERFUNCTION_HPP_INCLUDE
#define UITSL_META_HASMEMBERFUNCTION_HPP_INCLUDE

#include <type_traits>

// adapted from https://stackoverflow.com/a/16824239
#define UITSL_GENERATE_HAS_MEMBER_FUNCTION( MEMBER ) \
  /* Primary template with a static assertion */ \
  /* for a meaningful error message */ \
  /* if it ever gets instantiated. */ \
  /* We could leave it undefined if we didn't care. */ \
  template<typename, typename T> \
  struct HasMemberFunction_##MEMBER { \
    static_assert( \
      std::integral_constant<T, false>::value, \
      "Second template parameter needs to be of function type." \
    ); \
  }; \
  /* specialization that does the checking */ \
  template<typename C, typename Ret, typename... Args> \
  class HasMemberFunction_##MEMBER<C, Ret(Args...)> { \
    template<typename T> \
    static constexpr auto check(T*) \
    -> typename \
    std::is_same< \
      decltype( std::declval<T>().MEMBER( std::declval<Args>()... ) ), \
      Ret    /* ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ */ \
    >::type; /* attempt to call it and see if the return type is correct */ \
    template<typename> \
    static constexpr std::false_type check(...); \
    typedef decltype(check<C>(0)) type; \
  public: \
    static constexpr bool value = type::value; \
  };

#endif // #ifndef UITSL_META_HASMEMBERFUNCTION_HPP_INCLUDE
