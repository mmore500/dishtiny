#pragma once
#ifndef UIT_SETUP_IMPLSPEC_HPP_INCLUDE
#define UIT_SETUP_IMPLSPEC_HPP_INCLUDE

#include "defaults.hpp"
#include "ImplSelect.hpp"

namespace uit {

namespace internal {

template<
  typename T_,
  typename ImplSelect,
  size_t N_,
  size_t B_
>
class ImplSpecKernel {

  /// TODO.
  using THIS_T = ImplSpecKernel<T_, ImplSelect, N_, B_>;

public:

  /// TODO.
  using T = T_;

  /// TODO.
  constexpr inline static size_t N{ N_ };

  /// TODO.
  constexpr inline static size_t B{ B_ };

  /// TODO.
  using IntraDuct = typename ImplSelect::template IntraDuct<THIS_T>;

  /// TODO.
  using ThreadDuct = typename ImplSelect::template ThreadDuct<THIS_T>;

  /// TODO.
  using ProcInletDuct = typename ImplSelect::template
    ProcDuct<THIS_T>::InletImpl;

  /// TODO.
  using ProcOutletDuct = typename ImplSelect::template
    ProcDuct<THIS_T>::OutletImpl;

  /// TODO.
  using ProcBackEnd = typename ImplSelect::template
    ProcDuct<THIS_T>::BackEndImpl;

  // TODO add static ToString

};

} // namespace internal

/**
 * Specifies implementation details for the conduit framework.
 *
 * @tparam T Type to transmit.
 * @tparam ImplSelect Class with static typedef members specifying which
 * implementations to use for intra-thread, inter-thread, and inter-process
 * transmission.
 * @tparam N Buffer size.
 * @tparam B For buffered or aggregated ducts,
 * maximum number of items to buffer.
 *
 */
template<
  typename T,
  typename ImplSelect=uit::ImplSelect<>,
  template<typename> typename SpoutWrapper=uit::DefaultSpoutWrapper,
  size_t N=uit::DEFAULT_BUFFER,
  size_t B=std::numeric_limits<size_t>::max(),
  size_t SpoutCacheSize_=2
>
class ImplSpec
: public internal::ImplSpecKernel<
  typename SpoutWrapper<T>::T,
  ImplSelect, N, B
> {

  using wrapper_t = SpoutWrapper<T>;

public:

  using value_type = T;

  template<typename Inlet>
  using inlet_wrapper_t = typename wrapper_t::template inlet_wrapper_t<Inlet>;

  template<typename Outlet>
  using outlet_wrapper_t
    = typename wrapper_t::template outlet_wrapper_t<Outlet>;

  constexpr inline static size_t SpoutCacheSize{ SpoutCacheSize_ };

};

template<typename T>
struct MockSpec
: public ImplSpec<T, uit::MockSelect>
{};

} // namespace uit

#endif // #ifndef UIT_SETUP_IMPLSPEC_HPP_INCLUDE
