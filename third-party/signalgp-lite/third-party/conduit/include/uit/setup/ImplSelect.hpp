#pragma once
#ifndef UIT_SETUP_IMPLSELECT_HPP_INCLUDE
#define UIT_SETUP_IMPLSELECT_HPP_INCLUDE

#include "defaults.hpp"

namespace uit {

/**
 * Specifies the `Duct` implementations to be used for intra-thread, inter-
 * thread, and inter-process transmission.
 *
 * @tparam IntraDuct_ Implementation to use for intra-thread transmission.
 * @tparam ThreadDuct_ Implementation to use for inter-thread transmission.
 * @tparam ProcDuct_ Implementation to use for inter-process transmission
 */
template<
  template<typename> typename IntraDuct_ = uit::DefaultIntraDuct,
  template<typename> typename ThreadDuct_ = uit::DefaultThreadDuct,
  template<typename> typename ProcDuct_ = uit::DefaultProcDuct
>
struct ImplSelect {

  template<typename ImplSpec>
  using IntraDuct = IntraDuct_<ImplSpec>;

  template<typename ImplSpec>
  using ThreadDuct = ThreadDuct_<ImplSpec>;

  template<typename ImplSpec>
  using ProcDuct = ProcDuct_<ImplSpec>;

};

struct MockSelect
: public ImplSelect<
  uit::DefaultMockDuct,
  uit::DefaultMockDuct,
  uit::DefaultMockDuct
> {};

} // namespace uit

#endif // #ifndef UIT_SETUP_IMPLSELECT_HPP_INCLUDE
