#include "uit/ducts/mock/ThrowDuct.hpp"
#include "uit/ducts/thread/accumulating+type=fundamental/f::CompareExchangeDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

using ImplSel = uit::ImplSelect<
  uit::f::CompareExchangeDuct,
  uit::f::CompareExchangeDuct,
  uit::ThrowDuct
>;

#define IMPL_NAME "f::CompareExchangeDuct"

#include "../ThreadDuct.hpp"

#include "../AccumulatingThreadDuct.hpp"
