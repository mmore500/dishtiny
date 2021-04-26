#include "uit/ducts/proc/put=growing+get=stepping+type=trivial/inlet=DequeIrsend+outlet=RingIrecv_t::IdirOriDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::a::AtomicPendingDuct,
  uit::t::IdirOriDuct
>;

#include "../ProcDuct.hpp"
