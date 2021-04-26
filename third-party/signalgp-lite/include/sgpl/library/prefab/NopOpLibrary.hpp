#pragma once
#ifndef SGPL_LIBRARY_PREFAB_NOPOPLIBRARY_HPP_INCLUDE
#define SGPL_LIBRARY_PREFAB_NOPOPLIBRARY_HPP_INCLUDE

#include "../../operations/operations.hpp"

#include "../OpLibrary.hpp"

namespace sgpl {

using NopOpLibrary = sgpl::OpLibrary<
  sgpl::Nop<>
>;

} // namespace sgpl

#endif // #ifndef SGPL_LIBRARY_PREFAB_NOPOPLIBRARY_HPP_INCLUDE
