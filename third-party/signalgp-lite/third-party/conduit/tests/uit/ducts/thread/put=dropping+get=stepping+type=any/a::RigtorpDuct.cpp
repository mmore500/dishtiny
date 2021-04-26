#include "uit/ducts/mock/ThrowDuct.hpp"
#include "uit/ducts/thread/put=dropping+get=stepping+type=any/a::RigtorpDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::RigtorpDuct,
  uit::a::RigtorpDuct,
  uit::ThrowDuct
>;

#define IMPL_NAME "a::RigtorpDuct"

#include "../ThreadDuct.hpp"

#include "../SteppingThreadDuct.hpp"
#include "../ValueThreadDuct.hpp"
