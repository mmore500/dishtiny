#include "uit/ducts/mock/ThrowDuct.hpp"
#include "uit/ducts/thread/accumulating+type=fundamental/f::AtomicAccumulatingDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

using ImplSel = uit::ImplSelect<
  uit::f::AtomicAccumulatingDuct,
  uit::f::AtomicAccumulatingDuct,
  uit::ThrowDuct
>;

#define IMPL_NAME "f::AtomicAccumulatingDuct"

#include "../ThreadDuct.hpp"

#include "../AccumulatingThreadDuct.hpp"
