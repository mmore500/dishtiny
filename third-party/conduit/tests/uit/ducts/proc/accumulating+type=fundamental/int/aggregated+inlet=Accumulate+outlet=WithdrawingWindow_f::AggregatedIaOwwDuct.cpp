#include "uit/ducts/intra/accumulating+type=any/a::AccumulatingDuct.hpp"
#include "uit/ducts/mock/ThrowDuct.hpp"
#include "uit/ducts/proc/accumulating+type=fundamental/aggregated+inlet=Accumulate+outlet=WithdrawingWindow_f::AggregatedIaOwwDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::AccumulatingDuct,
  uit::ThrowDuct,
  uit::f::AggregatedIaOwwDuct
>;

using MSG_T = int;
#define IMPL_NAME "aggregated+inlet=Accumulate+outlet=WithdrawingWindow_f::AggregatedIaOwwDuct/int"

#include "../../AccumulatingProcDuct.hpp"
