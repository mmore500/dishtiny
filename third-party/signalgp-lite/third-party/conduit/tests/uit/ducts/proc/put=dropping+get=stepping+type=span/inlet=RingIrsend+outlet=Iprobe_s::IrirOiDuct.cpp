#include "uit/ducts/mock/ThrowDuct.hpp"
#include "uit/ducts/proc/put=dropping+get=stepping+type=span/inlet=RingIrsend+outlet=Iprobe_s::IrirOiDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::ThrowDuct,
  uit::s::IrirOiDuct
>;

#define IMPL_NAME "inlet=RingIrsend+outlet=Iprobe_s::IrirOiDuct"

#include "../ProcDuct.hpp"
#include "../VectorProcDuct.hpp"
