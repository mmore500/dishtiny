#pragma once
#ifndef DISH2_SPEC_GENOMEMESHSPEC_HPP_INCLUDE
#define DISH2_SPEC_GENOMEMESHSPEC_HPP_INCLUDE

#include "../../../third-party/conduit/include/uit/ducts/intra/put=growing+get=skipping+type=any/a::SconceDuct.hpp"
#include "../../../third-party/conduit/include/uit/ducts/mock/EmpAssertDuct.hpp"
#include "../../../third-party/conduit/include/uit/ducts/proc/put=dropping+get=stepping+type=cereal/aggregated+inlet=RingIsend+outlet=Iprobe_c::AggregatedIriOiDuct.hpp"
#include "../../../third-party/conduit/include/uit/ducts/thread/put=growing+get=skipping+type=any/a::MutexSconceDuct.hpp"
#include "../../../third-party/conduit/include/uit/setup/ImplSelect.hpp"
#include "../../../third-party/conduit/include/uit/setup/ImplSpec.hpp"
#include "../../../third-party/conduit/include/uit/spouts/wrappers/CachingSpoutWrapper.hpp"

#include "../genome/Genome.hpp"

namespace dish2 {

namespace internal_genome_spec {

  using ImplSel = uit::ImplSelect<
    uit::a::SconceDuct,
    #ifndef __EMSCRIPTEN__
      uit::a::MutexSconceDuct
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

  template<typename Spec>
  using ImplSpec = uit::ImplSpec<
    dish2::Genome<Spec>,
    ImplSel,
    uit::CachingSpoutWrapper,
    2, // N
    16 // B
  >;

} // namespace internal_genome_spec

template<typename Spec>
using GenomeMeshSpec = internal_genome_spec::ImplSpec<Spec>;

} // namespace dish2

#endif // #ifndef DISH2_SPEC_GENOMEMESHSPEC_HPP_INCLUDE
