#pragma once
#ifndef UIT_DUCTS_PROC_IMPL_OUTLET_ACCUMULATING_TYPE_SPANFUNDAMENTAL_SF__WITHDRAWINGWINDOWDUCT_HPP_INCLUDE
#define UIT_DUCTS_PROC_IMPL_OUTLET_ACCUMULATING_TYPE_SPANFUNDAMENTAL_SF__WITHDRAWINGWINDOWDUCT_HPP_INCLUDE

#include <algorithm>
#include <array>
#include <limits>
#include <stddef.h>

#include <mpi.h>

#include "../../../../../../../third-party/Empirical/include/emp/base/always_assert.hpp"
#include "../../../../../../../third-party/Empirical/include/emp/base/assert.hpp"
#include "../../../../../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../../../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "../../../../../../uitsl/debug/safe_compare.hpp"
#include "../../../../../../uitsl/debug/WarnOnce.hpp"
#include "../../../../../../uitsl/distributed/RdmaAccumulatorBundle.hpp"
#include "../../../../../../uitsl/distributed/RdmaWindowManager.hpp"
#include "../../../../../../uitsl/meta/f::static_test.hpp"
#include "../../../../../../uitsl/meta/s::static_test.hpp"
#include "../../../../../../uitsl/mpi/mpi_utils.hpp"
#include "../../../../../../uitsl/nonce/CircularIndex.hpp"
#include "../../../../../../uitsl/utility/print_utils.hpp"

#include "../../../../../setup/InterProcAddress.hpp"

#include "../../backend/RuntimeSizeBackEnd.hpp"
#include "../../backend/RuntimeSizeRdmaBackEnd.hpp"

namespace uit {
namespace sf {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
class WithdrawingWindowDuct {

public:

  using BackEndImpl = uit::RuntimeSizeRdmaBackEnd<ImplSpec>;

private:

  using T = typename ImplSpec::T;
  using value_type = typename T::value_type;
  static_assert( uitsl::s::static_test<T>(), uitsl_s_message );
  static_assert(
    uitsl::f::static_test<value_type>(), uitsl_f_message
  );
  constexpr inline static size_t N{ImplSpec::N};

  const uit::InterProcAddress address;

  std::shared_ptr<BackEndImpl> back_end;

  using bundle_t = uitsl::RdmaAccumulatorBundle<value_type>;
  bundle_t cache;
  bundle_t pristine;

  const int byte_offset;

  size_t CountUnconsumedGets() const { return 1; }

public:

  WithdrawingWindowDuct(
    const uit::InterProcAddress& address_,
    std::shared_ptr<BackEndImpl> back_end_,
    const uit::RuntimeSizeBackEnd<ImplSpec>& rts
      =uit::RuntimeSizeBackEnd<ImplSpec>{}
  ) : address(address_)
  , back_end(back_end_)
  , cache( rts.HasSize() ? rts.GetSize() : back_end->GetSize() )
  , pristine( rts.HasSize() ? rts.GetSize() : back_end->GetSize() )
  , byte_offset(
    address.GetOutletProc() == uitsl::get_rank(address.GetComm())
    ? back_end->GetWindowManager().Acquire(
      address.GetInletProc(),
      emp::vector<std::byte>(
        reinterpret_cast<std::byte*>(pristine.data()),
        reinterpret_cast<std::byte*>(pristine.data()) + pristine.byte_size()
      )
    ) : -1
  ) {
    emp_assert( rts.HasSize() || back_end->HasSize() );
    if (address.GetOutletProc() == uitsl::get_rank(address.GetComm())) {
      MPI_Request req;
      UITSL_Isend(
        &byte_offset, // const void *buf
        1, // int count
        MPI_INT, // MPI_Datatype datatype
        address.GetInletProc(), // int dest
        address.GetTag(), // int tag
        address.GetComm(), // MPI_Comm comm
        &req // MPI_Request * request
      );
      UITSL_Request_free(&req); //TODO test for completion in destructor?
    }

  }

  [[noreturn]] bool TryPut(const T&) {
    emp_always_assert(false, "TryPut called on WithdrawingWindowDuct");
    __builtin_unreachable();
  }

  [[noreturn]] bool TryFlush() const {
    emp_always_assert(false, "Flush called on WithdrawingWindowDuct");
    __builtin_unreachable();
  }

  size_t TryConsumeGets(const size_t requested) {
    emp_assert( requested == std::numeric_limits<size_t>::max() );

    // lock own window
    back_end->GetWindowManager().LockShared( address.GetInletProc() );

    std::byte *target = back_end->GetWindowManager().GetBytes(
      address.GetInletProc(),
      byte_offset
    );

    // in case cache has been moved from or epoch got popped off
    // TODO this can be optimized to remove unnecessary operations
    cache.Reset();

    // get data out of window
    std::memcpy(
      reinterpret_cast<std::byte*>(cache.data()),
      target,
      cache.byte_size()
    );

    // reset data in window
    std::memcpy(
      target,
      reinterpret_cast<const std::byte*>(pristine.data()),
      cache.byte_size()
    );

    back_end->GetWindowManager().Unlock( address.GetInletProc() );

    emp_assert( cache.GetEpoch() >= 0 );

    return static_cast<size_t>( cache.GetEpoch() );
  }

  const T& Get() const { return cache.GetData(); }

  T& Get() { return cache.GetData(); }

  static std::string GetName() { return "WithdrawingWindowDuct"; }

  static constexpr bool CanStep() { return false; }

  std::string ToString() const {
    std::stringstream ss;
    ss << GetName() << '\n';
    ss << uitsl::format_member("this", static_cast<const void *>(this)) << '\n';
    ss << uitsl::format_member("int uitsl::get_rank()", uitsl::get_rank()) << '\n';
    return ss.str();
  }

};

} // namespace f
} // namespace uit

#endif // #ifndef UIT_DUCTS_PROC_IMPL_OUTLET_ACCUMULATING_TYPE_SPANFUNDAMENTAL_SF__WITHDRAWINGWINDOWDUCT_HPP_INCLUDE
