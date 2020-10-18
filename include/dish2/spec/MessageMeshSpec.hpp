#pragma once
#ifndef DISH2_SPEC_MESSAGEMESHSPEC_HPP_INCLUDE
#define DISH2_SPEC_MESSAGEMESHSPEC_HPP_INCLUDE

#include "../../../third-party/conduit/include/uit/ducts/proc/put=dropping+get=stepping+type=cereal/aggregated+inlet=RingIsend+outlet=Iprobe_c::AggregatedIriOiDuct.hpp"
#include "../../../third-party/cereal/include/cereal/types/array.hpp"
#include "../../../third-party/conduit/include/uit/setup/ImplSelect.hpp"
#include "../../../third-party/conduit/include/uit/setup/ImplSpec.hpp"
#include "../../../third-party/Empirical/source/tools/BitSet.h"

namespace dish2 {

namespace internal_message_spec {

  using ImplSel = uit::ImplSelect<
    uit::DefaultIntraDuct,
    uit::DefaultThreadDuct,
    uit::c::AggregatedIriOiDuct
  >;

  using ImplSpec = uit::ImplSpec<
    emp::BitSet<32>, // FIXME
    ImplSel,
    uit::DefaultSpoutWrapper,
    2, // N
    16 // B
  >;

} // namespace internal_message_spec

using MessageMeshSpec = internal_message_spec::ImplSpec;

} // namespace dish2

#endif // #ifndef DISH2_SPEC_MESSAGEMESHSPEC_HPP_INCLUDE
