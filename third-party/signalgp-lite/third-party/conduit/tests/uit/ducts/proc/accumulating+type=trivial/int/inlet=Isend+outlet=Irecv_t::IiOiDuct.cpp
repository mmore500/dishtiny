#include "uit/ducts/intra/accumulating+type=any/a::AccumulatingDuct.hpp"
#include "uit/ducts/mock/ThrowDuct.hpp"
#include "uit/ducts/proc/accumulating+type=trivial/inlet=Isend+outlet=Irecv_t::IiOiDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::AccumulatingDuct,
  uit::ThrowDuct,
  uit::t::IiOiDuct
>;

using MSG_T = int;
#define IMPL_NAME "inlet=Isend+outlet=Irecv_t::IiOiDuct/int"
#define TAGS "[nproc:2][nproc:3][nproc:4]"

#include "../../AccumulatingProcDuct.hpp"
