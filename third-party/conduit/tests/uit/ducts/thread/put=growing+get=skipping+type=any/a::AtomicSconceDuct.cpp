#include "uit/ducts/mock/ThrowDuct.hpp"
#include "uit/ducts/thread/put=growing+get=skipping+type=any/a::AtomicSconceDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::AtomicSconceDuct,
  uit::a::AtomicSconceDuct,
  uit::ThrowDuct
>;

#define IMPL_NAME "a::AtomicSconceDuct"

#include "../ThreadDuct.hpp"

// TODO FIXME fix segfault
// #include "../ValueThreadDuct.hpp"
// #include "../SkippingThreadDuct.hpp"
