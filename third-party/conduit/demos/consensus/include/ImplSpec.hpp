#pragma once

#include "uit/ducts/intra/put=growing+get=skipping+type=any/a::SconceDuct.hpp"
#include "uit/ducts/proc/put=dropping+get=skipping+type=trivial/pooled+inlet=RingIsend+outlet=BlockIrecv_t::PooledIriObiDuct.hpp"
// #include "uit/ducts/proc/put=dropping+get=skipping+type=trivial/inlet=RingIsend+outlet=BlockIrecv_t::IriObiDuct.hpp"
#include "uit/ducts/thread/put=growing+get=skipping+type=any/a::MutexSconceDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::SconceDuct,
  uit::a::MutexSconceDuct,
  // uit::t::IriObiDuct
  uit::t::PooledIriObiDuct
>;

using ImplSpec = uit::ImplSpec<
  message_t,
  ImplSel,
  uit::DefaultSpoutWrapper,
  2, // N
  16 // B
>;
