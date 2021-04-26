#include "uit/ducts/mock/ThrowDuct.hpp"
#include "uit/ducts/thread/accumulating+type=any/a::MutexAccumulatingDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::MutexAccumulatingDuct,
  uit::a::MutexAccumulatingDuct,
  uit::ThrowDuct
>;

#define IMPL_NAME "a::MutexAccumulatingDuct"

#include "../ThreadDuct.hpp"

#include "../AccumulatingThreadDuct.hpp"
