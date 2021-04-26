#include "uit/ducts/proc/put=dropping+get=stepping+type=trivial/inlet=RingIsend+outlet=RingIrecv_t::IriOriDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::a::AtomicPendingDuct,
  uit::t::IriOriDuct
>;

#include "../ProcDuct.hpp"
