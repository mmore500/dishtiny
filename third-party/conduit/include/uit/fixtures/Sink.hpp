#pragma once
#ifndef UIT_FIXTURES_SINK_HPP_INCLUDE
#define UIT_FIXTURES_SINK_HPP_INCLUDE

#include <memory>
#include <stddef.h>
#include <utility>

#include "../ducts/Duct.hpp"
#include "../spouts/Inlet.hpp"

namespace uit {

/**
 * Creates an uncoupled `Inlet`.
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
class Sink {

  template<typename Inlet>
  using inlet_wrapper_t = typename ImplSpec::template inlet_wrapper_t<Inlet>;

public:

  using inlet_t = inlet_wrapper_t< uit::Inlet<ImplSpec> >;

private:

  /// `Sink`'s `Inlet`.
  inlet_t inlet;

public:

  /**
   * Copy constructor.
   */
  Sink(Sink& other) = default;

  /**
   * Copy constructor.
   */
  Sink(const Sink& other) = default;

  /**
   * Move constructor.
   */
  Sink(Sink&& other) = default;

  /**
   * Forwarding constructor.
   *
   * Use `std::in_place_type_t<ImplType>{}` followed by constructor arguments to
   * initialize the `Duct` with `ImplType` active.
   */
  template <typename... Args>
  Sink(Args&&... args) : inlet(
    std::make_shared<internal::Duct<ImplSpec>>(
      std::forward<Args>(args)...
    )
  ) { ; }

  /**
   * Adaptor for structured bindings as interface to access `Sink`'s `Inlet`.
   *
   * @tparam N Index of element to access, 0 for `Inlet`.
   * @return `Sink`'s `Inlet`.
   */
  template <size_t N>
  decltype(auto) get() const { if constexpr (N == 0) return inlet; }

  /**
   * Accessor for `Sink`'s `Inlet`.
   *
   * @return `Sink`'s `Inlet`.
   */
  inlet_t& GetInlet() { return inlet; }

};

} // namespace uit

namespace std {

  /// Adaptor for structured bindings.
  template<typename ImplSpec>
  struct tuple_size<uit::Sink<ImplSpec>>
    : std::integral_constant<size_t, 1>{};

  /// Adaptor for structured bindings.
  template<typename ImplSpec, size_t N>
  struct tuple_element<N, uit::Sink<ImplSpec>> {
    using type = decltype(
      std::declval<uit::Sink<ImplSpec>>().template get<N>()
    );
  };

} // namespace std

#endif // #ifndef UIT_FIXTURES_SINK_HPP_INCLUDE
