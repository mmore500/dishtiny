#include "uit/ducts/intra/accumulating+type=any/a::AccumulatingDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::AccumulatingDuct
>;

#define MSG_T double
#define IMPL_NAME "a::AccumulatingDuct/double"

#include "../../IntraDuct.hpp"

#include "../../AccumulatingIntraDuct.hpp"
