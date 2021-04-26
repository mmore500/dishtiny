#include "uit/ducts/thread/put=growing+get=skipping+type=any/a::AtomicSconceDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::AtomicSconceDuct,
  uit::a::AtomicSconceDuct
>;

#include "../ThreadDuct.hpp"
