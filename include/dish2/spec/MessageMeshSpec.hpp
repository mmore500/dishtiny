#pragma once
#ifndef DISH2_SPEC_MESSAGEMESHSPEC_HPP_INCLUDE
#define DISH2_SPEC_MESSAGEMESHSPEC_HPP_INCLUDE

#include "../../../third-party/cereal/include/cereal/types/array.hpp"
#include "../../../third-party/cereal/include/cereal/types/tuple.hpp"
#include "../../../third-party/conduit/include/uit/ducts/mock/EmpAssertDuct.hpp"
#include "../../../third-party/conduit/include/uit/ducts/proc/put=dropping+get=stepping+type=cereal/aggregated+inlet=RingIsend+outlet=Iprobe_c::AggregatedIriOiDuct.hpp"
#include "../../../third-party/conduit/include/uit/setup/ImplSelect.hpp"
#include "../../../third-party/conduit/include/uit/setup/ImplSpec.hpp"
#include "../../../third-party/Empirical/include/emp/bits/BitSet.hpp"

namespace dish2 {

namespace internal_message_spec {

  using ImplSel = uit::ImplSelect<
    uit::DefaultIntraDuct,
    #ifndef __EMSCRIPTEN__
      uit::DefaultThreadDuct
    #else
      uit::EmpAssertDuct
    #endif
    ,
    #ifndef __EMSCRIPTEN__
      uit::c::AggregatedIriOiDuct
    #else
      uit::EmpAssertDuct
    #endif
  >;

  template< typename Spec >
  using ImplSpec = uit::ImplSpec<
    typename Spec::message_t,
    ImplSel,
    uit::DefaultSpoutWrapper,
    2, // N
    16 // B
  >;

} // namespace internal_message_spec

template< typename Spec >
using MessageMeshSpec = internal_message_spec::ImplSpec< Spec >;

} // namespace dish2

#endif // #ifndef DISH2_SPEC_MESSAGEMESHSPEC_HPP_INCLUDE
