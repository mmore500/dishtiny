#pragma once

#include "uit/ducts/intra/accumulating+type=any/a::AccumulatingDuct.hpp"
#include "uit/ducts/proc/accumulating+type=fundamental/aggregated+inlet=Raccumulate+outlet=WithdrawingWindow_f::AggregatedIrOwwDuct.hpp"
#include "uit/ducts/proc/accumulating+type=trivial/aggregated+inlet=Isend+outlet=Irecv_t::AggregatedIiOiDuct.hpp"
#include "uit/ducts/proc/accumulating+type=trivial/inlet=Isend+outlet=Irecv_t::IiOiDuct.hpp"
#include "uit/ducts/thread/accumulating+type=any/a::MutexAccumulatingDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::AccumulatingDuct,
  uit::a::MutexAccumulatingDuct,
  uit::t::AggregatedIiOiDuct
>;

using ImplSpec = uit::ImplSpec<
  message_t,
  ImplSel,
  uit::DefaultSpoutWrapper,
  2, // N
  16 // B
>;
