#include "uit/ducts/mock/ThrowDuct.hpp"
#include "uit/ducts/proc/put=dropping+get=skipping+type=span/inlet=RingIrsend+outlet=BlockIrecv_s::IrirObiDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

using ImplSel = uit::ImplSelect<
  uit::ThrowDuct,
  uit::ThrowDuct,
  uit::IrirObi
>;

#define TAGS "[nproc:2][nproc:3][nproc:4]"

#include "../FixedVectorProcDuct.hpp"
