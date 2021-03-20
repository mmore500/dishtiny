#pragma once
#ifndef DISH2_SPEC_PUSHMESHSPEC_HPP_INCLUDE
#define DISH2_SPEC_PUSHMESHSPEC_HPP_INCLUDE

#include "../../../third-party/cereal/include/cereal/types/array.hpp"
#include "../../../third-party/cereal/include/cereal/types/tuple.hpp"
#include "../../../third-party/conduit/include/uit/ducts/intra/put=growing+get=stepping+type=any/a::DequeDuct.hpp"
#include "../../../third-party/conduit/include/uit/ducts/mock/EmpAssertDuct.hpp"
#include "../../../third-party/conduit/include/uit/ducts/proc/put=growing+get=stepping+type=cereal/aggregated+inlet=DequeIsend+outlet=Iprobe_c::AggregatedIdiOiDuct.hpp"
#include "../../../third-party/conduit/include/uit/ducts/thread/put=growing+get=stepping+type=any/a::UnboundedMoodyCamelDuct.hpp"
#include "../../../third-party/conduit/include/uit/setup/ImplSelect.hpp"
#include "../../../third-party/conduit/include/uit/setup/ImplSpec.hpp"
#include "../../../third-party/Empirical/include/emp/bits/BitSet.hpp"

namespace dish2 {

namespace internal_push_spec {

  using ImplSel = uit::ImplSelect<
    uit::a::DequeDuct,
    #ifndef __EMSCRIPTEN__
      uit::a::UnboundedMoodyCamelDuct
    #else
      uit::EmpAssertDuct
    #endif
    ,
    #ifndef __EMSCRIPTEN__
      uit::c::AggregatedIdiOiDuct
    #else
      uit::EmpAssertDuct
    #endif
  >;

  template< typename Spec >
  using ImplSpec = uit::ImplSpec<
    typename Spec::push_message_t,
    ImplSel,
    uit::DefaultSpoutWrapper,
    2, // N
    16 // B
  >;

} // namespace internal_push_spec

template< typename Spec >
using PushMeshSpec = internal_push_spec::ImplSpec< Spec >;

} // namespace dish2

#endif // #ifndef DISH2_SPEC_PUSHMESHSPEC_HPP_INCLUDE
