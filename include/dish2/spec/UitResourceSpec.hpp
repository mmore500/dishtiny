#pragma once
#ifndef DISH2_SPEC_UITRESOURCESPEC_HPP_INCLUDE
#define DISH2_SPEC_UITRESOURCESPEC_HPP_INCLUDE

#include "../../../third-party/conduit/include/uit/ducts/proc/put=dropping+get=stepping+type=cereal/aggregated+inlet=RingIsend+outlet=Iprobe_c::AggregatedIriOiDuct.hpp"
#include "../../../third-party/conduit/include/uit/setup/ImplSpec.hpp"
#include "../../../third-party/conduit/include/uit/setup/ImplSelect.hpp"

namespace dish2 {

namespace internal_resource_spec {
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

}

using UitResourceSpec = internal_resource_spec::ImplSpec;

}

#endif // #ifndef DISH2_SPEC_UITRESOURCESPEC_HPP_INCLUDE
