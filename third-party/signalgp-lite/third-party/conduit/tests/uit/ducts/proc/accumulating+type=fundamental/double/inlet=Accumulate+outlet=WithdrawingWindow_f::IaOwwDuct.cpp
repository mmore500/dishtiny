#include "uit/ducts/intra/accumulating+type=any/a::AccumulatingDuct.hpp"
#include "uit/ducts/mock/ThrowDuct.hpp"
#include "uit/ducts/proc/accumulating+type=fundamental/inlet=Accumulate+outlet=WithdrawingWindow_f::IaOwwDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::AccumulatingDuct,
  uit::ThrowDuct,
  uit::f::IaOwwDuct
>;

using MSG_T = double;
#define IMPL_NAME "inlet=Accumulate+outlet=WithdrawingWindow_f::IaOwwDuct/double"

#include "../../AccumulatingProcDuct.hpp"
