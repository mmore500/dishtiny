#pragma once
#ifndef SGPL_LIBRARY_PREFAB_ARITHMETICOPLIBRARY_HPP_INCLUDE
#define SGPL_LIBRARY_PREFAB_ARITHMETICOPLIBRARY_HPP_INCLUDE

#include "../../operations/operations.hpp"

#include "../OpLibrary.hpp"

namespace sgpl {

using ArithmeticOpLibrary = sgpl::OpLibrary<
  sgpl::Add,
  sgpl::Divide,
  sgpl::Multiply,
  sgpl::Subtract//,
  // sgpl::Modulo,
  // sgpl::Terminal<>
>;

} // namespace sgpl

#endif // #ifndef SGPL_LIBRARY_PREFAB_ARITHMETICOPLIBRARY_HPP_INCLUDE
