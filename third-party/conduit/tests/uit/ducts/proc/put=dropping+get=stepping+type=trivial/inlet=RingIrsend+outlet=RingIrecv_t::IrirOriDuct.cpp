#include "uit/ducts/mock/ThrowDuct.hpp"
#include "uit/ducts/proc/put=dropping+get=stepping+type=trivial/inlet=RingIrsend+outlet=RingIrecv_t::IrirOriDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::ThrowDuct,
  uit::t::IrirOriDuct
>;

#define IMPL_NAME "inlet=RingIrsend+outlet=RingIrecv_t::IrirOriDuct"
#define TAGS "[nproc:2][nproc:3][nproc:4]"

#include "../ProcDuct.hpp"
#include "../SkippingProcDuct.hpp"
