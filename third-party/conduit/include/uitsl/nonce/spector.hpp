#pragma once
#ifndef UITSL_NONCE_SPECTOR_HPP_INCLUDE
#define UITSL_NONCE_SPECTOR_HPP_INCLUDE

#include <variant>

#include "../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../third-party/Empirical/include/emp/polyfill/span.hpp"

namespace uitsl {

template<typename T>
class spector {

  using vector_t = emp::vector<T>;
  using span_t = std::span<T>;

  std::variant<vector_t, span_t> impl;

public:

  /**
   * Forwarding constructor.
   *
   */
  template <typename S, typename... Args>
  spector(std::in_place_type_t<S> which, Args&&... args)
  : impl(
    which,
    std::forward<Args>(args)...
  )
  { ; }

  /**
   * Forwarding constructor.
   *
   */
  template <typename... Args>
  explicit spector(Args&&... args)
  : impl(
    std::in_place_type_t<vector_t>{},
    std::forward<Args>(args)...
  )
  { ; }

  /**
   * TODO.
   *
   * @return TODO.
   */
  operator vector_t&() {
    emp_assert( std::holds_alternative<vector_t>(impl) );
    return std::get<vector_t>(impl);
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  operator const vector_t&() const {
    emp_assert( std::holds_alternative<vector_t>(impl) );
    return std::get<vector_t>(impl);
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  operator span_t&() {
    emp_assert( std::holds_alternative<span_t>(impl) );
    return std::get<span_t>(impl);
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  operator const span_t&() const {
    emp_assert( std::holds_alternative<span_t>(impl) );
    return std::get<span_t>(impl);
  }


  /**
   * TODO.
   *
   * @return TODO.
   */
  T *data() {
    return std::visit(
      [](auto& arg) -> T* { return arg.data(); },
      impl
    );
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  const T *data() const {
    return std::visit(
      [](const auto& arg) -> const T* { return arg.data(); },
      impl
    );
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  size_t size() const {
    return std::visit(
      [](const auto& arg) -> size_t { return arg.size(); },
      impl
    );
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  size_t size_bytes() const {
    return std::visit(
      [](const auto& arg) -> size_t { return arg.size() * sizeof(T); },
      impl
    );
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  void resize(const size_t count) {
    emp_assert( std::holds_alternative<vector_t>(impl) );
    std::get<emp::vector<T>>(impl).resize(count);
  }



};

} // namespace uitsl

#endif // #ifndef UITSL_NONCE_SPECTOR_HPP_INCLUDE
