#pragma once
#ifndef UIT_FIXTURES_CONDUIT_HPP_INCLUDE
#define UIT_FIXTURES_CONDUIT_HPP_INCLUDE

#include <memory>
#include <stddef.h>
#include <utility>

#include "../ducts/Duct.hpp"
#include "../spouts/Inlet.hpp"
#include "../spouts/Outlet.hpp"

namespace uit {

/**
 * Creates a coupled `Inlet` and `Outlet`.
 *
 * Useful for intra-process transmission (i.e., intra-thread or inter-thread
 * transmission) where conceptually-coupled `Inlet`'s and `Outlet`'s exist
 * within the same memory space.
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework. See
 *   `include/config/ImplSpec.hpp`.
 */
template<typename ImplSpec>
class Conduit {

  /// Typedef for `Duct` implementation.
  using duct_t = uit::internal::Duct<ImplSpec>;

  /// Handle to `Duct` shared by `inlet` and `outlet`.
  std::shared_ptr<duct_t> duct;

  template<typename Inlet>
  using inlet_wrapper_t = typename ImplSpec::template inlet_wrapper_t<Inlet>;
  template<typename Outlet>
  using outlet_wrapper_t = typename ImplSpec::template outlet_wrapper_t<Outlet>;

public:

  using inlet_t = inlet_wrapper_t< uit::Inlet<ImplSpec> >;
  using outlet_t = outlet_wrapper_t< uit::Outlet<ImplSpec> >;

private:

  /// `Conduit`'s `Inlet`.
  inlet_t inlet;
  /// `Conduit`'s `Outlet`.
  outlet_t outlet;

public:

  /**
   * Copy constructor.
   */
  Conduit(Conduit& other) = default;

  /**
   * Copy constructor.
   */
  Conduit(const Conduit& other) = default;

  /**
   * Move constructor.
   */
  Conduit(Conduit&& other) = default;

  /**
   * Forwarding constructor.
   *
   * Use `std::in_place_type_t<ImplType>{}` followed by constructor arguments to
   * initialize the `Duct` with `ImplType` active.
   */
  template <typename... Args>
  Conduit(Args&&... args) : duct(
    std::make_shared<internal::Duct<ImplSpec>>( std::forward<Args>(args)... )
  ), inlet(duct)
  , outlet(duct)
  { ; }

  /**
   * Adaptor for structured bindings as interface to access `Conduit`'s `Inlet`
   * or `Outlet`.
   *
   * @tparam N Index of element to access, 0 for `Inlet` or 1 for `Outlet`.
   * @return `Conduit`'s `Inlet` or `Outlet`.
   */
  template <size_t N>
  decltype(auto) get() const {
    if constexpr (N == 0) return inlet;
    else if constexpr (N == 1) return outlet;
  }

  /**
   * Accessor for `Conduit`'s `Inlet`.
   *
   * @return `Conduit`'s `Inlet`.
   */
  inlet_t& GetInlet() { return inlet; }

  /**
   * Accessor for `Conduit`'s `Outlet`.
   *
   * @return `Conduit`'s `Inlet`.
   */
  outlet_t& GetOutlet() { return outlet; }

};

} // namespace uit

namespace std {

  /// Adaptor for structured bindings.
  template<typename ImplSpec>
  struct tuple_size<uit::Conduit<ImplSpec>>
    : std::integral_constant<size_t, 2>{};

  /// Adaptor for structured bindings.
  template<typename ImplSpec, size_t N>
  struct tuple_element<N, uit::Conduit<ImplSpec>> {
    using type = decltype(
      std::declval<uit::Conduit<ImplSpec>>().template get<N>()
    );
  };

} // namespace std

#endif // #ifndef UIT_FIXTURES_CONDUIT_HPP_INCLUDE
