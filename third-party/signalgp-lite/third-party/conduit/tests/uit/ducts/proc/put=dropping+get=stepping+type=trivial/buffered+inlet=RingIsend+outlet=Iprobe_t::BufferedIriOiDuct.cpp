#include "uit/ducts/mock/ThrowDuct.hpp"
#include "uit/ducts/proc/put=dropping+get=stepping+type=trivial/buffered+inlet=RingIsend+outlet=Iprobe_t::BufferedIriOiDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SerialPendingDuct,
  uit::ThrowDuct,
  uit::t::BufferedIriOiDuct
>;

#define IMPL_NAME "buffered+inlet=RingIsend+outlet=Iprobe_t::BufferedIriOiDuct"
#define TAGS "[nproc:2][nproc:3][nproc:4]"

#include "../ProcDuct.hpp"
#include "../SteppingProcDuct.hpp"
