#pragma once
#ifndef UIT_DUCTS_PROC_IMPL_OUTLET_ACCUMULATING_TYPE_FUNDAMENTAL_F__WITHDRAWINGWINDOWDUCT_HPP_INCLUDE
#define UIT_DUCTS_PROC_IMPL_OUTLET_ACCUMULATING_TYPE_FUNDAMENTAL_F__WITHDRAWINGWINDOWDUCT_HPP_INCLUDE

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
#include "../../../../../../uitsl/distributed/RdmaAccumulatorPacket.hpp"
#include "../../../../../../uitsl/distributed/RdmaWindowManager.hpp"
#include "../../../../../../uitsl/meta/f::static_test.hpp"
#include "../../../../../../uitsl/mpi/mpi_utils.hpp"
#include "../../../../../../uitsl/nonce/CircularIndex.hpp"
#include "../../../../../../uitsl/utility/print_utils.hpp"

#include "../../../../../setup/InterProcAddress.hpp"

#include "../../backend/RdmaBackEnd.hpp"

namespace uit {
namespace f {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
class WithdrawingWindowDuct {

public:

  using BackEndImpl = uit::RdmaBackEnd<ImplSpec>;

private:

  using T = typename ImplSpec::T;
  static_assert( uitsl::f::static_test<T>(), uitsl_f_message );
  constexpr inline static size_t N{ImplSpec::N};

  using packet_t = uitsl::RdmaAccumulatorPacket<T>;

  packet_t cache{};

  const uit::InterProcAddress address;

  std::shared_ptr<BackEndImpl> back_end;
  const int byte_offset;

  size_t CountUnconsumedGets() const { return 1; }

public:

  WithdrawingWindowDuct(
    const uit::InterProcAddress& address_,
    std::shared_ptr<BackEndImpl> back_end_
  ) : address(address_)
  , back_end(back_end_)
  , byte_offset(
    address.GetOutletProc() == uitsl::get_rank(address.GetComm())
      ? back_end->GetWindowManager().Acquire(
        address.GetInletProc(),
        emp::vector<std::byte>(
          reinterpret_cast<std::byte*>(&cache),
          reinterpret_cast<std::byte*>(&cache) + sizeof(packet_t)
        )
      ) : -1
  ) {
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

    // get data out of window
    std::memcpy(
      reinterpret_cast<std::byte*>(&cache),
      target,
      sizeof(packet_t)
    );

    emp_assert( reinterpret_cast<packet_t&>(*target).data == cache.data );

    // reset data in window
    const static packet_t pristine{};
    std::memcpy(
      target,
      reinterpret_cast<const std::byte*>(&pristine),
      sizeof(packet_t)
    );

    emp_assert( reinterpret_cast<packet_t&>(*target).data == T{} );

    back_end->GetWindowManager().Unlock( address.GetInletProc() );

    emp_assert( cache.epoch >= 0 );

    return static_cast<size_t>( cache.epoch );
  }

  const T& Get() const { return cache.data; }

  T& Get() { return cache.data; }

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

#endif // #ifndef UIT_DUCTS_PROC_IMPL_OUTLET_ACCUMULATING_TYPE_FUNDAMENTAL_F__WITHDRAWINGWINDOWDUCT_HPP_INCLUDE
