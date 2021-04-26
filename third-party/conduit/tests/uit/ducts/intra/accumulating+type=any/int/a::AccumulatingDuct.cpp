#include "uit/ducts/intra/accumulating+type=any/a::AccumulatingDuct.hpp"
#include "uit/setup/ImplSpec.hpp"

using ImplSel = uit::ImplSelect<
  uit::a::AccumulatingDuct
>;

#define MSG_T int
#define IMPL_NAME "a::AccumulatingDuct/int"

#include "../../IntraDuct.hpp"

#include "../../AccumulatingIntraDuct.hpp"
