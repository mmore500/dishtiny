#include "uit/ducts/intra/put=dropping+get=stepping+type=any/a::SerialPendingDuct.hpp"
#include "uit/ducts/mock/ThrowDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::ThrowDuct,
  uit::ThrowDuct
>;

#define MSG_T int
#define IMPL_NAME "a::SerialPendingDuct/MSG_T"

#include "../IntraDuct.hpp"
#include "../SteppingIntraDuct.hpp"
#include "../ValueIntraDuct.hpp"
