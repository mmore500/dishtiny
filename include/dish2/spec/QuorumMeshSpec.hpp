#pragma once
#ifndef DISH2_SPEC_QUORUMMESHSPEC_HPP_INCLUDE
#define DISH2_SPEC_QUORUMMESHSPEC_HPP_INCLUDE

#include "../../../third-party/conduit/include/uit/ducts/intra/put=growing+get=skipping+type=any/a::SconceDuct.hpp"
#include "../../../third-party/conduit/include/uit/ducts/mock/EmpAssertDuct.hpp"
#include "../../../third-party/conduit/include/uit/ducts/proc/put=dropping+get=skipping+type=trivial/pooled+inlet=RingIsend+outlet=BlockIrecv_t::PooledIriObiDuct.hpp"
#include "../../../third-party/conduit/include/uit/ducts/thread/put=growing+get=skipping+type=any/a::MutexSconceDuct.hpp"
#include "../../../third-party/conduit/include/uit/setup/ImplSelect.hpp"
#include "../../../third-party/conduit/include/uit/setup/ImplSpec.hpp"

#include "../quorum/QuorumMessage.hpp"

namespace dish2 {

namespace internal_quorum_spec {

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
  dish2::QuorumMessage<Spec>,
  ImplSel,
  uit::DefaultSpoutWrapper,
  1, // N
  0 // B
>;

} // namespace internal_quorum_spec

template<typename Spec>
using QuorumMeshSpec = internal_quorum_spec::template ImplSpec<Spec>;

} // namespace dish2

#endif // #ifndef DISH2_SPEC_QUORUMMESHSPEC_HPP_INCLUDE
