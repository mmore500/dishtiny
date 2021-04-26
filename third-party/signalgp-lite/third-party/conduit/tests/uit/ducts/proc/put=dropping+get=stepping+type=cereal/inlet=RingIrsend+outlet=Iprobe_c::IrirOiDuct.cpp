#include "uit/ducts/mock/ThrowDuct.hpp"
#include "uit/ducts/proc/put=dropping+get=stepping+type=cereal/inlet=RingIsend+outlet=Iprobe_c::IrirOiDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::ThrowDuct,
  uit::c::IriOiDuct
>;

#define IMPL_NAME "inlet=RingIsend+outlet=Iprobe_c::IrirOiDuct"

#include "../ProcDuct.hpp"
#include "../SkippingProcDuct.hpp"
