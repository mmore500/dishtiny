#include "uit/ducts/mock/ThrowDuct.hpp"
#include "uit/ducts/proc/put=dropping+get=skipping+type=trivial/inlet=RingRput+outlet=Window_t::IrrOwDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

#include "uitsl/mpi/mpi_guard.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::ThrowDuct,
  uit::t::IrrOwDuct
>;

#define IMPL_NAME "inlet=RingRput+outlet=Window_t::IrrOwDuct"
#define TAGS "[nproc:2][nproc:3][nproc:4][nproc:5][nproc:6][nproc:7][nproc:8]"

// TODO why does this hang on travis?
// #include "../ProcDuct.hpp"
// #include "../SkippingProcDuct.hpp"
