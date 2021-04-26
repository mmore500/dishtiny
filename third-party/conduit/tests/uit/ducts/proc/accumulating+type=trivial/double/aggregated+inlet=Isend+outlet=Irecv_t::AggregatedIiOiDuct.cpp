#include "uit/ducts/intra/accumulating+type=any/a::AccumulatingDuct.hpp"
#include "uit/ducts/mock/ThrowDuct.hpp"
#include "uit/ducts/proc/accumulating+type=trivial/aggregated+inlet=Isend+outlet=Irecv_t::AggregatedIiOiDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::AccumulatingDuct,
  uit::ThrowDuct,
  uit::t::AggregatedIiOiDuct
>;

using MSG_T = double;
#define IMPL_NAME "aggregated+inlet=Isend+outlet=Irecv_t::AggregatedIiOiDuct/double"
#define TAGS "[nproc:2][nproc:3][nproc:4]"

#include "../../AccumulatingProcDuct.hpp"
