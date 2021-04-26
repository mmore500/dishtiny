#include "uit/ducts/proc/put=growing+get=stepping+type=cereal/inlet=Send+outlet=Iprobe_c::IsOiDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::a::AtomicPendingDuct,
  uit::c::IsOiDuct
>;

#include "../ProcDuct.hpp"
