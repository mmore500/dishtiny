#include "uit/ducts/thread/put=growing+get=skipping+type=any/a::MutexSconceDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::a::MutexSconceDuct
>;

#include "../ThreadDuct.hpp"
