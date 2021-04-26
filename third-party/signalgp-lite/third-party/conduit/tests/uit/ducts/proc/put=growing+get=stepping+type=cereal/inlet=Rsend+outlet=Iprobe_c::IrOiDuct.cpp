#include "uit/ducts/mock/ThrowDuct.hpp"
#include "uit/ducts/proc/put=growing+get=stepping+type=cereal/inlet=Rsend+outlet=Iprobe_c::IrOiDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::ThrowDuct,
  uit::c::IrOiDuct
>;

#define IMPL_NAME "inlet=Rsend+outlet=Iprobe_c::IrOiDuct"
#define TAGS "[nproc:2][nproc:3][nproc:4][nproc:5][nproc:6][nproc:7][nproc:8]"

#include "../ProcDuct.hpp"
#include "../SteppingProcDuct.hpp"
