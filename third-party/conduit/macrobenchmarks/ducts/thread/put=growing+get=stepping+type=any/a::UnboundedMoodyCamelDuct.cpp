#include "uit/ducts/thread/put=dropping+get=stepping+type=any/a::BoundedMoodyCamelDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::a::BoundedMoodyCamelDuct
>;

#include "../ThreadDuct.hpp"
