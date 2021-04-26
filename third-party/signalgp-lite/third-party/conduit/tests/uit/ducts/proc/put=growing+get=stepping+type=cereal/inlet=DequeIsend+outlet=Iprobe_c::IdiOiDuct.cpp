#include "uit/ducts/mock/ThrowDuct.hpp"
#include "uit/ducts/proc/put=growing+get=stepping+type=cereal/inlet=DequeIsend+outlet=Iprobe_c::IdiOiDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::ThrowDuct,
  uit::c::IdiOiDuct
>;

#define IMPL_NAME "inlet=DequeIsend+outlet=Iprobe_c::IdiOiDuct"
#define TAGS "[nproc:2][nproc:3][nproc:4][nproc:5][nproc:6][nproc:7][nproc:8]"

#include "../ProcDuct.hpp"
#include "../SteppingProcDuct.hpp"
