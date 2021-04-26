#include "uit/ducts/mock/ThrowDuct.hpp"
#include "uit/ducts/proc/accumulating+type=spanfundamental/inlet=Raccumulate+outlet=WithdrawingWindow_sf::IrOwwDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

using ImplSel = uit::ImplSelect<
  uit::ThrowDuct,
  uit::ThrowDuct,
  uit::sf::IrOwwDuct
>;

#define MSG_VALUE_T double
#define IMPL_NAME "inlet=Raccumulate+outlet=WithdrawingWindow_sf::IrOwwDuct/double"

#include "../../VectorAccumulatingProcDuct.hpp"
