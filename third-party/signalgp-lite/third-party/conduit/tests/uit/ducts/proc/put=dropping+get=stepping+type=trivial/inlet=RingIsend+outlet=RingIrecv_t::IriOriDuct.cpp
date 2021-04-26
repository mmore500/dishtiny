#include "uit/ducts/mock/ThrowDuct.hpp"
#include "uit/ducts/proc/put=dropping+get=stepping+type=trivial/inlet=RingIsend+outlet=RingIrecv_t::IriOriDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::ThrowDuct,
  uit::t::IriOriDuct
>;

#define IMPL_NAME "inlet=RingIsend+outlet=RingIrecv_t::IriOriDuct"
#define TAGS "[nproc:2][nproc:3][nproc:4]"

#include "../ProcDuct.hpp"
#include "../SkippingProcDuct.hpp"
