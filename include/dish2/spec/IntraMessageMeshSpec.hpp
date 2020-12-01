#pragma once
#ifndef DISH2_SPEC_INTRAMESSAGEMESHSPEC_HPP_INCLUDE
#define DISH2_SPEC_INTRAMESSAGEMESHSPEC_HPP_INCLUDE

#include "../../../third-party/cereal/include/cereal/types/array.hpp"
#include "../../../third-party/conduit/include/uit/ducts/mock/EmpAssertDuct.hpp"
#include "../../../third-party/conduit/include/uit/setup/ImplSelect.hpp"
#include "../../../third-party/conduit/include/uit/setup/ImplSpec.hpp"
#include "../../../third-party/Empirical/include/emp/bits/BitSet.hpp"

namespace dish2 {

namespace internal_intra_message_spec {

  using ImplSel = uit::ImplSelect<
    uit::DefaultIntraDuct,
    uit::EmpAssertDuct,
    uit::EmpAssertDuct
  >;

  template< typename Spec >
  using ImplSpec = uit::ImplSpec<
    typename Spec::message_t,
    ImplSel,
    uit::DefaultSpoutWrapper,
    4, // N
    4 // B
  >;

} // namespace internal_intra_message_spec

template< typename Spec >
using IntraMessageMeshSpec = internal_intra_message_spec::ImplSpec< Spec >;

} // namespace dish2

#endif // #ifndef DISH2_SPEC_INTRAMESSAGEMESHSPEC_HPP_INCLUDE
