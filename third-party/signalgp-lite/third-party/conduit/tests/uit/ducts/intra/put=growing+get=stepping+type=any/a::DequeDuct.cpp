#include "uit/ducts/intra/put=growing+get=stepping+type=any/a::DequeDuct.hpp"
#include "uit/ducts/mock/ThrowDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::DequeDuct,
  uit::ThrowDuct,
  uit::ThrowDuct
>;

#define MSG_T int
#define IMPL_NAME "a::DequeDuct/MSG_T"

#include "../IntraDuct.hpp"
#include "../SteppingIntraDuct.hpp"
#include "../ValueIntraDuct.hpp"
