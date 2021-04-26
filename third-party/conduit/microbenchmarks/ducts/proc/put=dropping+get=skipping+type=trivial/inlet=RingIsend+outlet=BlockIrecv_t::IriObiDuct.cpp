#include "uit/ducts/proc/put=dropping+get=skipping+type=trivial/inlet=RingIsend+outlet=BlockIrecv_t::IriObiDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::a::AtomicPendingDuct,
  uit::t::IriObiDuct
>;

#include "../ProcDuct.hpp"
