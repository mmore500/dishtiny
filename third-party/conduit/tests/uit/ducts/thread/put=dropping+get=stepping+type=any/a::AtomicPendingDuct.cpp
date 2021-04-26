#include "uit/ducts/mock/ThrowDuct.hpp"
#include "uit/ducts/thread/put=dropping+get=stepping+type=any/a::AtomicPendingDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::AtomicPendingDuct,
  uit::a::AtomicPendingDuct,
  uit::ThrowDuct
>;

#define IMPL_NAME "a::AtomicPendingDuct"

#include "../ThreadDuct.hpp"

#include "../SteppingThreadDuct.hpp"
#include "../ValueThreadDuct.hpp"
