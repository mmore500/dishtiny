#include "uit/ducts/mock/ThrowDuct.hpp"
#include "uit/ducts/proc/put=growing+get=skipping+type=trivial/inlet=Send+outlet=BlockIrecv_t::IsObiDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::ThrowDuct,
  uit::t::IsObiDuct
>;

#define IMPL_NAME "inlet=Send+outlet=BlockIrecv_t::IsObiDuct"
#define TAGS "[nproc:2][nproc:3][nproc:4]"

#include "../ProcDuct.hpp"
#include "../SkippingProcDuct.hpp"
