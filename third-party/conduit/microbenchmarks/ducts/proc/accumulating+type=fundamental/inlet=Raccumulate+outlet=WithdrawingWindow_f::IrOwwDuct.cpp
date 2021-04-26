#include "uit/ducts/proc/accumulating+type=fundamental/inlet=Raccumulate+outlet=WithdrawingWindow_f::IrOwwDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::a::AtomicPendingDuct,
  uit::f::IrOwwDuct
>;

#include "../ProcDuct.hpp"
