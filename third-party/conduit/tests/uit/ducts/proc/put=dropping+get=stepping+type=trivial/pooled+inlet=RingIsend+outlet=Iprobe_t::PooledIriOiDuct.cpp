#include "uit/ducts/mock/ThrowDuct.hpp"
#include "uit/ducts/proc/put=dropping+get=stepping+type=trivial/pooled+inlet=RingIsend+outlet=Iprobe_t::PooledIriOiDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::ThrowDuct,
  uit::t::PooledIriOiDuct
>;

#define IMPL_NAME "pooled+inlet=RingIsend+outlet=Iprobe_t::PooledIriOiDuct"
#define TAGS "[nproc:2][nproc:3][nproc:4]"

#include "../ProcDuct.hpp"
#include "../SteppingProcDuct.hpp"
