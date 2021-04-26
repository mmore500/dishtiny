#pragma once

#include "uit/ducts/proc/put=dropping+get=stepping+type=cereal/aggregated+inlet=RingIsend+outlet=Iprobe_c::AggregatedIriOiDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

using ImplSel = uit::ImplSelect<
  uit::DefaultIntraDuct,
  uit::DefaultThreadDuct,
  uit::c::AggregatedIriOiDuct
>;

using ImplSpec = uit::ImplSpec<
  message_t,
  ImplSel,
  uit::DefaultSpoutWrapper,
  2, // N
  16 // B
>;
