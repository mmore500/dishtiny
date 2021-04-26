#include "uit/ducts/intra/put=dropping+get=stepping+type=any/a::HeadTailDuct.hpp"
#include "uit/ducts/mock/ThrowDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::HeadTailDuct,
  uit::ThrowDuct,
  uit::ThrowDuct
>;

#define MSG_T int
#define IMPL_NAME "a::HeadTailDuct/MSG_T"

#include "../IntraDuct.hpp"
#include "../SteppingIntraDuct.hpp"
#include "../ValueIntraDuct.hpp"
