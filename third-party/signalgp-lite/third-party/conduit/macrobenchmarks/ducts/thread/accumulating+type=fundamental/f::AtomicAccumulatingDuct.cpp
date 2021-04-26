#include "uit/ducts/thread/accumulating+type=fundamental/f::AtomicAccumulatingDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::f::AtomicAccumulatingDuct
>;

#include "../ThreadDuct.hpp"
