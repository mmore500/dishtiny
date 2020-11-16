#pragma once
#ifndef DISH2_OPERATIONS_OPLIBRARY_HPP_INCLUDE
#define DISH2_OPERATIONS_OPLIBRARY_HPP_INCLUDE

#include "../../../third-party/signalgp-lite/include/sgpl/library/OpLibraryCoupler.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/library/prefab/CompleteOpLibrary.hpp"

#include "AddToOwnState.hpp"
#include "BcstIntraMessageIf.hpp"
#include "MultiplyOwnState.hpp"
#include "ReadNeighborState.hpp"
#include "ReadOwnState.hpp"
#include "SendInterMessageIf.hpp"
#include "SendIntraMessageIf.hpp"
#include "WriteOwnStateIf.hpp"

namespace dish2 {

template< typename DishSpec >
using OpLibrary = sgpl::OpLibraryCoupler<
  sgpl::CompleteOpLibrary,
  dish2::AddToOwnState< DishSpec >,
  dish2::BcstIntraMessageIf,
  dish2::MultiplyOwnState< DishSpec >,
  dish2::ReadNeighborState,
  dish2::ReadOwnState,
  dish2::SendInterMessageIf,
  dish2::SendIntraMessageIf,
  dish2::WriteOwnStateIf< DishSpec >
>;

} // namespace dish2

#endif // #ifndef DISH2_OPERATIONS_OPLIBRARY_HPP_INCLUDE
