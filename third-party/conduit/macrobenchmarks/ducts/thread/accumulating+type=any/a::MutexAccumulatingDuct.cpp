#include "uit/ducts/thread/accumulating+type=any/a::MutexAccumulatingDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::MutexAccumulatingDuct,
  uit::a::MutexAccumulatingDuct
>;

#include "../ThreadDuct.hpp"
