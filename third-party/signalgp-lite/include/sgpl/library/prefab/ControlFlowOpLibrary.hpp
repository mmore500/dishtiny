#pragma once
#ifndef SGPL_LIBRARY_PREFAB_CONTROLFLOWOPLIBRARY_HPP_INCLUDE
#define SGPL_LIBRARY_PREFAB_CONTROLFLOWOPLIBRARY_HPP_INCLUDE

#include "../../operations/operations.hpp"

#include "../OpLibrary.hpp"

namespace sgpl {

using ControlFlowOpLibrary = sgpl::OpLibrary<
  sgpl::global::Anchor,
  sgpl::global::JumpIf,
  sgpl::global::JumpIfNot,
  sgpl::global::RegulatorAdj<>,
  sgpl::global::RegulatorGet<>,
  sgpl::global::RegulatorSet<>,
  sgpl::local::Anchor,
  sgpl::local::JumpIf,
  sgpl::local::JumpIfNot,
  sgpl::local::RegulatorAdj,
  sgpl::local::RegulatorGet,
  sgpl::local::RegulatorSet
>;

} // namespace sgpl

#endif // #ifndef SGPL_LIBRARY_PREFAB_CONTROLFLOWOPLIBRARY_HPP_INCLUDE
