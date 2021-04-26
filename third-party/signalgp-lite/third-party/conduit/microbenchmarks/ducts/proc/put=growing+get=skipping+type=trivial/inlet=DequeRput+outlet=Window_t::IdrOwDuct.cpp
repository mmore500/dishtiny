#include "uit/ducts/proc/put=growing+get=skipping+type=trivial/inlet=DequeRput+outlet=Window_t::IdrOwDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::a::AtomicPendingDuct,
  uit::t::IdrOwDuct
>;

#include "../ProcDuct.hpp"
