#pragma once
#ifndef UIT_DUCTS_PROC_IMPL_INLET_ACCUMULATING_TYPE_FUNDAMENTAL_F__ACCUMULATEDUCT_HPP_INCLUDE
#define UIT_DUCTS_PROC_IMPL_INLET_ACCUMULATING_TYPE_FUNDAMENTAL_F__ACCUMULATEDUCT_HPP_INCLUDE

#include <algorithm>
#include <array>
#include <stddef.h>
#include <string>

#include <mpi.h>

#include "../../../../../../../third-party/Empirical/include/emp/base/always_assert.hpp"
#include "../../../../../../../third-party/Empirical/include/emp/base/assert.hpp"
#include "../../../../../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../../../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "../../../../../../uitsl/debug/WarnOnce.hpp"
#include "../../../../../../uitsl/distributed/RdmaAccumulatorPacket.hpp"
#include "../../../../../../uitsl/distributed/RdmaWindowManager.hpp"
#include "../../../../../../uitsl/meta/f::static_test.hpp"
#include "../../../../../../uitsl/mpi/mpi_utils.hpp"
#include "../../../../../../uitsl/mpi/Request.hpp"
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
class AccumulateDuct {

public:

  using BackEndImpl = uit::RdmaBackEnd<ImplSpec>;

private:

  using T = typename ImplSpec::T;
  static_assert( uitsl::f::static_test<T>(), uitsl_f_message );
  constexpr inline static size_t N{ImplSpec::N};
  using packet_t = uitsl::RdmaAccumulatorPacket<T>;

  const uit::InterProcAddress address;

  std::shared_ptr<BackEndImpl> back_end;

  uitsl::Request target_offset_request;
  int target_offset;

  void DoAccumulate(const packet_t& packet) {

    // TODO FIXME what kind of lock is needed here?
    back_end->GetWindowManager().LockShared( address.GetOutletProc() );

    back_end->GetWindowManager().template Accumulate<T>(
      address.GetOutletProc(),
      reinterpret_cast<const std::byte*>( &packet ),
      sizeof(packet_t),
      target_offset
    );

    back_end->GetWindowManager().Unlock( address.GetOutletProc() );

  }

public:

  AccumulateDuct(
    const uit::InterProcAddress& address_,
    std::shared_ptr<BackEndImpl> back_end_
  ) : address(address_)
  , back_end(back_end_)
  {

    if (uitsl::get_rank(address.GetComm()) == address.GetInletProc()) {
      // make spoof call to ensure reciporical activation
      back_end->GetWindowManager().Acquire(
        address.GetOutletProc(),
        emp::vector<std::byte>{}
      );

      // we'll emp_assert later to make sure it actually completed
      UITSL_Irecv(
        &target_offset, // void *buf
        1, // int count
        MPI_INT, // MPI_Datatype datatype
        address.GetOutletProc(), // int source
        address.GetTag(), // int tag
        address.GetComm(), // MPI_Comm comm
        &target_offset_request // MPI_Request *request
      );
    }

  }

  /**
   * TODO.
   *
   * @param val TODO.
   */
  bool TryPut(const T& val) {
    DoAccumulate( packet_t(val, 1) );
    return true;
  }

  /**
   * TODO.
   *
   */
  bool TryFlush() const { return true; }

  [[noreturn]] size_t TryConsumeGets(size_t) const {
    emp_always_assert(false, "ConsumeGets called on AccumulateDuct");
    __builtin_unreachable();
  }

  [[noreturn]] const T& Get() const {
    emp_always_assert(false, "Get called on AccumulateDuct");
    __builtin_unreachable();
  }

  [[noreturn]] T& Get() {
    emp_always_assert(false, "Get called on AccumulateDuct");
    __builtin_unreachable();
  }

  static std::string GetType() { return "AccumulateDuct"; }

  std::string ToString() const {
    std::stringstream ss;
    ss << GetType() << '\n';
    ss << uitsl::format_member("this", static_cast<const void *>(this)) << '\n';
    ss << uitsl::format_member("InterProcAddress address", address) << '\n';
    return ss.str();
  }

};

} // namespace f
} // namespace uit

#endif // #ifndef UIT_DUCTS_PROC_IMPL_INLET_ACCUMULATING_TYPE_FUNDAMENTAL_F__ACCUMULATEDUCT_HPP_INCLUDE
