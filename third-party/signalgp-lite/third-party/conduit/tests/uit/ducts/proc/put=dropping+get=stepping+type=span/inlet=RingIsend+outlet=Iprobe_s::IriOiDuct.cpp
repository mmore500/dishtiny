#include "uit/ducts/mock/ThrowDuct.hpp"
#include "uit/ducts/proc/put=dropping+get=stepping+type=span/inlet=RingIsend+outlet=Iprobe_s::IriOiDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::ThrowDuct,
  uit::s::IriOiDuct
>;

#define IMPL_NAME "inlet=RingIsend+outlet=Iprobe_s::IriOiDuct"

#include "../VectorProcDuct.hpp"
