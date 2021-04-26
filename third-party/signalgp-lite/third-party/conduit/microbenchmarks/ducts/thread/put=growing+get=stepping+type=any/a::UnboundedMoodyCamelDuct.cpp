#include "uit/ducts/thread/put=growing+get=stepping+type=any/a::UnboundedMoodyCamelDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::a::UnboundedMoodyCamelDuct
>;

#include "../ThreadDuct.hpp"
