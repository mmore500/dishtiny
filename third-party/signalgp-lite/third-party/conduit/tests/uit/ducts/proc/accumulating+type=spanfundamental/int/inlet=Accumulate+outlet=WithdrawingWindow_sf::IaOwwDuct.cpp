#include "uit/ducts/mock/ThrowDuct.hpp"
#include "uit/ducts/proc/accumulating+type=spanfundamental/inlet=Accumulate+outlet=WithdrawingWindow_sf::IaOwwDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

using ImplSel = uit::ImplSelect<
  uit::ThrowDuct,
  uit::ThrowDuct,
  uit::sf::IaOwwDuct
>;

#define MSG_VALUE_T int
#define IMPL_NAME "inlet=Accumulate+outlet=WithdrawingWindow_sf::IaOwwDuct/int"

#include "../../VectorAccumulatingProcDuct.hpp"
