#include "uit/ducts/proc/put=dropping+get=skipping+type=trivial/inlet=RingRput+outlet=Window_t::IrrOwDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::a::AtomicPendingDuct,
  uit::t::IrrOwDuct
>;

#include "../ProcDuct.hpp"
