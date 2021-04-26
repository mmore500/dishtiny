#include "uit/ducts/thread/put=dropping+get=stepping+type=any/a::AtomicPendingDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::a::AtomicPendingDuct
>;

#include "../ThreadDuct.hpp"
