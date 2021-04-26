#pragma once
#ifndef SGPL_LIBRARY_PREFAB_SANSREGULATIONOPLIBRARY_HPP_INCLUDE
#define SGPL_LIBRARY_PREFAB_SANSREGULATIONOPLIBRARY_HPP_INCLUDE

#include "../../operations/operations.hpp"

#include "../OpLibrary.hpp"

namespace sgpl {

using SansRegulationOpLibrary = sgpl::OpLibrary<
  sgpl::ForkIf,
  sgpl::Nop<>,
  sgpl::TerminateIf,
  sgpl::Add,
  sgpl::Divide,
  sgpl::Modulo,
  sgpl::Multiply,
  sgpl::Subtract,
  sgpl::Equal,
  sgpl::GreaterThan,
  sgpl::LessThan,
  sgpl::LogicalAnd,
  sgpl::LogicalOr,
  sgpl::NotEqual,
  sgpl::global::Anchor,
  sgpl::global::JumpIf,
  sgpl::global::JumpIfNot,
  sgpl::local::Anchor,
  sgpl::local::JumpIf,
  sgpl::local::JumpIfNot,
  sgpl::Decrement,
  sgpl::Increment,
  sgpl::Negate,
  sgpl::Not,
  sgpl::RandomBool,
  sgpl::RandomDraw,
  sgpl::Terminal
>;

} // namespace sgpl

#endif // #ifndef SGPL_LIBRARY_PREFAB_SANSREGULATIONOPLIBRARY_HPP_INCLUDE
