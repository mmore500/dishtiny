#include "uit/ducts/proc/put=dropping+get=skipping+type=trivial/inlet=RingIrsend+outlet=BlockIrecv_t::IrirObiDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::a::AtomicPendingDuct,
  uit::t::IrirObiDuct
>;

#include "../ProcDuct.hpp"
