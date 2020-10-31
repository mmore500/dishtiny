#pragma once
#ifndef DISH2_OPERATIONS_OPLIBRARY_HPP_INCLUDE
#define DISH2_OPERATIONS_OPLIBRARY_HPP_INCLUDE

#include "../../../third-party/signalgp-lite/include/sgpl/library/OpLibraryCoupler.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/library/prefab/CompleteOpLibrary.hpp"

#include "AddToOwnState.hpp"
#include "BcstIntraMessage.hpp"
#include "MultiplyOwnState.hpp"
#include "ReadNeighborState.hpp"
#include "ReadOwnState.hpp"
#include "SendInterMessage.hpp"
#include "SendIntraMessage.hpp"
#include "WriteOwnState.hpp"

namespace dish2 {

using OpLibrary = sgpl::OpLibraryCoupler<
  sgpl::CompleteOpLibrary,
  dish2::AddToOwnState,
  dish2::BcstIntraMessage,
  dish2::MultiplyOwnState,
  dish2::ReadNeighborState,
  dish2::ReadOwnState,
  dish2::SendInterMessage,
  dish2::SendIntraMessage,
  dish2::WriteOwnState
>;

} // namespace dish2

#endif // #ifndef DISH2_OPERATIONS_OPLIBRARY_HPP_INCLUDE
