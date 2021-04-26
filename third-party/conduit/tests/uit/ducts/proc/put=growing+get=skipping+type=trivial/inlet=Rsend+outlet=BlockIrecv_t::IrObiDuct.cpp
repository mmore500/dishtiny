#include "uit/ducts/mock/ThrowDuct.hpp"
#include "uit/ducts/proc/put=growing+get=skipping+type=trivial/t::BlockRmsgDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::ThrowDuct,
  uit::t::BlockRmsgDuct
>;

#define IMPL_NAME "t::BlockRmsgDuct"
#define TAGS "[nproc:2][nproc:3][nproc:4]"

#include "../ProcDuct.hpp"
#include "../SkippingProcDuct.hpp"
