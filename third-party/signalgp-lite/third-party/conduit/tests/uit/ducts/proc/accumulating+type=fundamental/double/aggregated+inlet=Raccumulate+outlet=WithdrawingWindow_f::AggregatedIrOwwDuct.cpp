#include "uit/ducts/intra/accumulating+type=any/a::AccumulatingDuct.hpp"
#include "uit/ducts/mock/ThrowDuct.hpp"
#include "uit/ducts/proc/accumulating+type=fundamental/aggregated+inlet=Raccumulate+outlet=WithdrawingWindow_f::AggregatedIrOwwDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::AccumulatingDuct,
  uit::ThrowDuct,
  uit::f::AggregatedIrOwwDuct
>;

using MSG_T = double;
#define IMPL_NAME "aggregated+inlet=Raccumulate+outlet=WithdrawingWindow_f::AggregatedIrOwwDuct/double"

#include "../../AccumulatingProcDuct.hpp"
