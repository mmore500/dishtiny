#include "uit/ducts/intra/accumulating+type=any/a::AccumulatingDuct.hpp"
#include "uit/ducts/mock/ThrowDuct.hpp"
#include "uit/ducts/proc/accumulating+type=fundamental/inlet=Raccumulate+outlet=WithdrawingWindow_f::IrOwwDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::AccumulatingDuct,
  uit::ThrowDuct,
  uit::f::IrOwwDuct
>;

using MSG_T = int;
#define IMPL_NAME "inlet=Raccumulate+outlet=WithdrawingWindow_f::IrOwwDuct/int"

#include "../../AccumulatingProcDuct.hpp"
