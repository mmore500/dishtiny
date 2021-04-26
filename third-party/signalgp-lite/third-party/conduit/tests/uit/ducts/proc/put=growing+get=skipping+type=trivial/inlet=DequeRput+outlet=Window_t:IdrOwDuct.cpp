#include "uit/ducts/mock/ThrowDuct.hpp"
#include "uit/ducts/proc/put=growing+get=skipping+type=trivial/inlet=DequeRput+outlet=Window_t::IdrOwDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::ThrowDuct,
  uit::t::IdrOwDuct
>;

#define IMPL_NAME "inlet=DequeRput+outlet=Window_t::IdrOwDuct"
#define TAGS "[nproc:2][nproc:3][nproc:4]"

// TODO why does this hang on travis
// #include "../ProcDuct.hpp"
// #include "../SkippingProcDuct.hpp"
