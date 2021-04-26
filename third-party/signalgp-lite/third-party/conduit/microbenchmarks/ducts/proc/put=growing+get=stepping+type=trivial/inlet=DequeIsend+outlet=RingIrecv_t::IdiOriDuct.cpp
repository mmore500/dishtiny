#include "uit/ducts/proc/put=growing+get=stepping+type=trivial/inlet=DequeIsend+outlet=RingIrecv_t::IdiOriDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::a::AtomicPendingDuct,
  uit::t::IdiOriDuct
>;

#include "../ProcDuct.hpp"
