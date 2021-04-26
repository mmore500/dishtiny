#include "uit/ducts/proc/put=growing+get=skipping+type=trivial/inlet=Send+outlet=BlockIrecv_t::IsObiDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::a::AtomicPendingDuct,
  uit::t::IsObiDuct
>;

#include "../ProcDuct.hpp"
