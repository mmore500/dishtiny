#include "uit/ducts/proc/put=growing+get=stepping+type=cereal/inlet=Rsend+outlet=Iprobe_c::IrOiDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::a::AtomicPendingDuct,
  uit::c::IdirOiDuct
>;

#include "../ProcDuct.hpp"
