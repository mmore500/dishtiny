#include "uit/ducts/mock/ThrowDuct.hpp"
#include "uit/ducts/proc/put=dropping+get=stepping+type=cereal/aggregated+inlet=RingIsend+outlet=Iprobe_c::AggregatedIriOiDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::ThrowDuct,
  uit::c::AggregatedIriOiDuct
>;

#define IMPL_NAME "aggregated+inlet=RingIsend+outlet=Iprobe_c::AggregatedIriOiDuct"

#include "../ProcDuct.hpp"
#include "../SteppingProcDuct.hpp"
