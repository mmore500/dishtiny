#pragma once
#ifndef DISH2_SPEC_RESOURCEMESHSPEC_HPP_INCLUDE
#define DISH2_SPEC_RESOURCEMESHSPEC_HPP_INCLUDE

#include "../../../third-party/conduit/include/uit/ducts/intra/accumulating+type=any/a::AccumulatingDuct.hpp"
#include "../../../third-party/conduit/include/uit/ducts/mock/EmpAssertDuct.hpp"
#include "../../../third-party/conduit/include/uit/ducts/proc/accumulating+type=fundamental/aggregated+inlet=Raccumulate+outlet=WithdrawingWindow_f::AggregatedIrOwwDuct.hpp"
#include "../../../third-party/conduit/include/uit/ducts/proc/accumulating+type=trivial/aggregated+inlet=Isend+outlet=Irecv_t::AggregatedIiOiDuct.hpp"
#include "../../../third-party/conduit/include/uit/ducts/proc/accumulating+type=trivial/inlet=Isend+outlet=Irecv_t::IiOiDuct.hpp"
#include "../../../third-party/conduit/include/uit/ducts/thread/accumulating+type=any/a::MutexAccumulatingDuct.hpp"
#include "../../../third-party/conduit/include/uit/setup/ImplSelect.hpp"
#include "../../../third-party/conduit/include/uit/setup/ImplSpec.hpp"

namespace dish2 {

namespace internal_resource_spec {

  using ImplSel = uit::ImplSelect<
    uit::a::AccumulatingDuct,
    #ifndef __EMSCRIPTEN__
      uit::a::MutexAccumulatingDuct
    #else
      uit::EmpAssertDuct
    #endif
    ,
    #ifndef __EMSCRIPTEN__
      uit::t::AggregatedIiOiDuct
    #else
      uit::EmpAssertDuct
    #endif
  >;

  using ImplSpec = uit::ImplSpec<
    float,
    ImplSel,
    uit::DefaultSpoutWrapper,
    2, // N
    16 // B
  >;

} // namespace internal_resource_spec

using ResourceMeshSpec = internal_resource_spec::ImplSpec;

} // namespace dish2

#endif // #ifndef DISH2_SPEC_RESOURCEMESHSPEC_HPP_INCLUDE
