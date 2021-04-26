#pragma once
#ifndef SGPL_LIBRARY_PREFAB_COMPLETEOPLIBRARY_HPP_INCLUDE
#define SGPL_LIBRARY_PREFAB_COMPLETEOPLIBRARY_HPP_INCLUDE

#include "../../operations/operations.hpp"

#include "../OpLibrary.hpp"

namespace sgpl {

using CompleteOpLibrary = sgpl::OpLibrary<
  sgpl::ForkIf,
  sgpl::Nop<0>,
  sgpl::Nop<1>,
  sgpl::Nop<2>,
  sgpl::TerminateIf,
  sgpl::Add,
  sgpl::Divide,
  sgpl::Modulo,
  sgpl::Multiply,
  sgpl::Subtract,
  sgpl::BitwiseAnd,
  sgpl::BitwiseNot,
  sgpl::BitwiseOr,
  sgpl::BitwiseShift,
  sgpl::BitwiseXor,
  sgpl::CountOnes,
  sgpl::RandomFill,
  sgpl::Equal,
  sgpl::GreaterThan,
  sgpl::LessThan,
  sgpl::LogicalAnd,
  sgpl::LogicalOr,
  sgpl::NotEqual,
  sgpl::global::Anchor,
  sgpl::global::JumpIf,
  sgpl::global::JumpIfNot,
  sgpl::global::RegulatorAdj<>,
  sgpl::global::RegulatorDecay<>,
  sgpl::global::RegulatorGet<>,
  sgpl::global::RegulatorSet<>,
  sgpl::local::Anchor,
  sgpl::local::JumpIf,
  sgpl::local::JumpIfNot,
  sgpl::local::RegulatorAdj,
  sgpl::local::RegulatorDecay,
  sgpl::local::RegulatorGet,
  sgpl::local::RegulatorSet,
  sgpl::Decrement,
  sgpl::Increment,
  sgpl::Negate,
  sgpl::Not,
  sgpl::RandomBool,
  sgpl::RandomDraw,
  sgpl::Terminal
>;

} // namespace sgpl

#endif // #ifndef SGPL_LIBRARY_PREFAB_COMPLETEOPLIBRARY_HPP_INCLUDE
