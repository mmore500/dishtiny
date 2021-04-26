#include "uit/ducts/proc/put=growing+get=stepping+type=trivial/inlet=Rsend+outlet=RingIrecv_t::IrOri.hpp"
#include "uit/setup/ImplSpec.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::a::AtomicPendingDuct,
  uit::t::IrOri
>;

#include "../ProcDuct.hpp"
