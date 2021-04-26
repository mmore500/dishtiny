#pragma once
#ifndef DISH2_SPEC_STATEMESHSPEC_HPP_INCLUDE
#define DISH2_SPEC_STATEMESHSPEC_HPP_INCLUDE

#include "../../../third-party/conduit/include/uit/ducts/intra/put=growing+get=skipping+type=any/a::SconceDuct.hpp"
#include "../../../third-party/conduit/include/uit/ducts/mock/EmpAssertDuct.hpp"
#include "../../../third-party/conduit/include/uit/ducts/proc/put=dropping+get=skipping+type=trivial/pooled+inlet=RingIsend+outlet=BlockIrecv_t::PooledIriObiDuct.hpp"
#include "../../../third-party/conduit/include/uit/ducts/thread/put=growing+get=skipping+type=any/a::MutexSconceDuct.hpp"
#include "../../../third-party/conduit/include/uit/setup/ImplSelect.hpp"
#include "../../../third-party/conduit/include/uit/setup/ImplSpec.hpp"

#include "../peripheral/readable_state/ReadableState.hpp"

namespace dish2 {

namespace internal_state_spec {

using ImplSel = uit::ImplSelect<
  uit::a::SconceDuct,
  #ifndef __EMSCRIPTEN__
    uit::a::MutexSconceDuct
  #else
    uit::EmpAssertDuct
  #endif
  ,
  #ifndef __EMSCRIPTEN__
    uit::t::PooledIriObiDuct
  #else
    uit::EmpAssertDuct
  #endif
>;

template<typename Spec>
using ImplSpec = uit::ImplSpec<
  dish2::ReadableState<Spec>,
  ImplSel,
  uit::DefaultSpoutWrapper,
  2, // N
  16 // B
>;

} // namespace internal_state_spec

template<typename Spec>
using StateMeshSpec = internal_state_spec::template ImplSpec<Spec>;

} // namespace dish2

#endif // #ifndef DISH2_SPEC_STATEMESHSPEC_HPP_INCLUDE
