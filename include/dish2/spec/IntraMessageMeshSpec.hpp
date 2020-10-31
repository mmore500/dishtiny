#pragma once
#ifndef DISH2_SPEC_INTRAMESSAGEMESHSPEC_HPP_INCLUDE
#define DISH2_SPEC_INTRAMESSAGEMESHSPEC_HPP_INCLUDE

#include "../../../third-party/cereal/include/cereal/types/array.hpp"
#include "../../../third-party/conduit/include/uit/ducts/mock/EmpAssertDuct.hpp"
#include "../../../third-party/conduit/include/uit/setup/ImplSelect.hpp"
#include "../../../third-party/conduit/include/uit/setup/ImplSpec.hpp"
#include "../../../third-party/Empirical/source/tools/BitSet.h"

namespace dish2 {

namespace internal_intra_message_spec {

  using ImplSel = uit::ImplSelect<
    uit::DefaultIntraDuct,
    uit::EmpAssertDuct,
    uit::EmpAssertDuct
  >;

  using ImplSpec = uit::ImplSpec<
    emp::BitSet<32>, // FIXME
    ImplSel,
    uit::DefaultSpoutWrapper,
    4, // N
    4 // B
  >;

} // namespace internal_intra_message_spec

using IntraMessageMeshSpec = internal_intra_message_spec::ImplSpec;

} // namespace dish2

#endif // #ifndef DISH2_SPEC_INTRAMESSAGEMESHSPEC_HPP_INCLUDE
