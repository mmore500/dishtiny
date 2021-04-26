#include "uit/ducts/proc/put=growing+get=stepping+type=cereal/inlet=DequeIsend+outlet=Iprobe_IdiOiDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

using ImplSel = uit::ImplSelect<
  uit::SerialPendingDuct,
  uit::AtomicPendingDuct,
  uit::IdiOiDuct
>;

#include "../ProcDuct.hpp"
