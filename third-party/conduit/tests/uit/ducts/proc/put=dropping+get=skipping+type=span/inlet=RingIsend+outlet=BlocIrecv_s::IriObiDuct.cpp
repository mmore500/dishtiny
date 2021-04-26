#include "uit/ducts/mock/ThrowDuct.hpp"
#include "uit/ducts/proc/put=dropping+get=skipping+type=span/inlet=RingIsend+outlet=BlockIrecv_s::IriObiDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

using ImplSel = uit::ImplSelect<
  uit::ThrowDuct,
  uit::ThrowDuct,
  uit::s::IriObiDuct
>;

#define TAGS "[nproc:2][nproc:3][nproc:4]"

#include "../FixedVectorProcDuct.hpp"
