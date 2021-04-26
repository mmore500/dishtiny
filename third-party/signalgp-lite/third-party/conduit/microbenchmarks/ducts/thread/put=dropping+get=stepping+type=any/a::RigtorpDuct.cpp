#include "uit/ducts/thread/put=dropping+get=stepping+type=any/a::RigtorpDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::a::RigtorpDuct
>;

#include "../ThreadDuct.hpp"
