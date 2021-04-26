#include "uit/ducts/mock/ThrowDuct.hpp"
#include "uit/ducts/proc/put=dropping+get=skipping+type=trivial/inlet=RingIsend+outlet=BlockIrecv_t::IriObiDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

#include "uitsl/mpi/mpi_guard.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::ThrowDuct,
  uit::t::IriObiDuct
>;

#define IMPL_NAME "inlet=RingIsend+outlet=BlockIrecv_t::IriObiDuct"
#define TAGS "[nproc:2][nproc:3][nproc:4][nproc:5][nproc:6][nproc:7][nproc:8]"

#include "../ProcDuct.hpp"
#include "../SkippingProcDuct.hpp"
