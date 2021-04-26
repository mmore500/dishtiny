#pragma once
#ifndef UIT_FIXTURES_SOURCE_HPP_INCLUDE
#define UIT_FIXTURES_SOURCE_HPP_INCLUDE

#include <memory>
#include <stddef.h>
#include <utility>

#include "../ducts/Duct.hpp"
#include "../spouts/Outlet.hpp"

namespace uit {

/**
 * Creates an ucoupled `Outlet`.
 *
 * Potentially useful for inter-process transmission, where a conceptually-
 * coupled `Inlet` and `Outlet` must be constructed within separate memory
 * spaces.
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework. See
 *   `include/config/ImplSpec.hpp`.
 */
template<typename ImplSpec>
class Source {

  template<typename Outlet>
  using outlet_wrapper_t = typename ImplSpec::template outlet_wrapper_t<Outlet>;

public:

  using outlet_t = outlet_wrapper_t< uit::Outlet<ImplSpec> >;

private:

  /// `Source`'s `Outlet`.
  outlet_t outlet;

public:

  /**
   * Copy constructor.
   */
  Source(Source& other) = default;

  /**
   * Copy constructor.
   */
  Source(const Source& other) = default;

  /**
   * Move constructor.
   */
  Source(Source&& other) = default;

  /**
   * Forwarding constructor.
   *
   * Use `std::in_place_type_t<ImplType>{}` followed by constructor arguments to
   * initialize the `Duct` with `ImplType` active.
   */
  template <typename... Args>
  Source(Args&&... args) : outlet(
    std::make_shared<internal::Duct<ImplSpec>>(
      std::forward<Args>(args)...
    )
  ) { ; }

  /**
   * Adaptor for structured bindings as interface to access `Source`'s `Outlet`.
   *
   * @tparam N Index of element to access, 0 for `Outlet`.
   * @return `Source`'s `Outlet`.
   */
  template <size_t N>
  decltype(auto) get() const { if constexpr (N == 0) return outlet; }

  /**
   * Accessor for `Source`'s `Outlet`.
   *
   * @return `Source`'s `Inlet`.
   */
  outlet_t& GetOutlet() { return outlet; }

};

} // namespace uit

namespace std {

  /// Adaptor for structured bindings.
  template<typename ImplSpec>
  struct tuple_size<uit::Source<ImplSpec>>
    : std::integral_constant<size_t, 1>{};

  /// Adaptor for structured bindings.
  template<typename ImplSpec, size_t N>
  struct tuple_element<N, uit::Source<ImplSpec>> {

    using type = decltype(
      std::declval<uit::Source<ImplSpec>>().template get<N>()
    );
  };

} // namespace std

#endif // #ifndef UIT_FIXTURES_SOURCE_HPP_INCLUDE
