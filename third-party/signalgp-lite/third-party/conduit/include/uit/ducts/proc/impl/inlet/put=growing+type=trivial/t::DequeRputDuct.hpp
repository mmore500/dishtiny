#pragma once
#ifndef UIT_DUCTS_PROC_IMPL_INLET_PUT_GROWING_TYPE_TRIVIAL_T__DEQUERPUTDUCT_HPP_INCLUDE
#define UIT_DUCTS_PROC_IMPL_INLET_PUT_GROWING_TYPE_TRIVIAL_T__DEQUERPUTDUCT_HPP_INCLUDE

#include <algorithm>
#include <array>
#include <deque>
#include <memory>
#include <stddef.h>
#include <tuple>

#include <mpi.h>

#include "../../../../../../../third-party/Empirical/include/emp/base/always_assert.hpp"
#include "../../../../../../../third-party/Empirical/include/emp/base/assert.hpp"
#include "../../../../../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../../../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "../../../../../../uitsl/debug/WarnOnce.hpp"
#include "../../../../../../uitsl/distributed/RdmaPacket.hpp"
#include "../../../../../../uitsl/distributed/RdmaWindowManager.hpp"
#include "../../../../../../uitsl/meta/t::static_test.hpp"
#include "../../../../../../uitsl/mpi/mpi_utils.hpp"
#include "../../../../../../uitsl/mpi/Request.hpp"
#include "../../../../../../uitsl/nonce/CircularIndex.hpp"
#include "../../../../../../uitsl/utility/print_utils.hpp"

#include "../../../../../setup/InterProcAddress.hpp"

#include "../../backend/RdmaBackEnd.hpp"

namespace uit {
namespace t {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
class DequeRputDuct {

public:

  using BackEndImpl = uit::RdmaBackEnd<ImplSpec>;

private:

  using T = typename ImplSpec::T;
  static_assert( uitsl::t::static_test<T>(), uitsl_t_message );
  constexpr inline static size_t N{ImplSpec::N};
  using packet_t = uitsl::RdmaPacket<T>;

  using buffer_t = std::deque<std::tuple<packet_t, uitsl::Request>>;
  buffer_t buffer;

  size_t epoch{};

  const uit::InterProcAddress address;

  std::shared_ptr<BackEndImpl> back_end;

  uitsl::Request target_offset_request;
  int target_offset;

  void PostPut() {

    // make sure that target offset has been received
    emp_assert( uitsl::test_completion(target_offset_request) );

    emp_assert( uitsl::test_null(
      std::get<uitsl::Request>(buffer.back())
    ) );

    // TODO FIXME what kind of lock is needed here?
    back_end->GetWindowManager().LockShared( address.GetOutletProc() );

    back_end->GetWindowManager().Rput(
      address.GetOutletProc(),
      reinterpret_cast<const std::byte*>( &std::get<packet_t>(buffer.back()) ),
      sizeof(packet_t),
      target_offset,
      &std::get<uitsl::Request>(buffer.back())
    );

    back_end->GetWindowManager().Unlock( address.GetOutletProc() );

    emp_assert( !uitsl::test_null(
      std::get<uitsl::Request>(buffer.back())
    ) );

  }

  bool TryFinalizePut() {
    emp_assert( !uitsl::test_null( std::get<uitsl::Request>(buffer.front()) ) );

    if (uitsl::test_completion( std::get<uitsl::Request>(buffer.front()) )) {
      emp_assert( uitsl::test_null( std::get<uitsl::Request>(buffer.front()) ) );
      buffer.pop_front();
      return true;
    } else return false;

  }

  void CancelPendingPut() {
    emp_assert( !uitsl::test_null( std::get<uitsl::Request>(buffer.front()) ) );

    UITSL_Cancel( &std::get<uitsl::Request>(buffer.front()) );
    UITSL_Request_free( &std::get<uitsl::Request>(buffer.front()) );

    emp_assert( uitsl::test_null( std::get<uitsl::Request>(buffer.front()) ) );

    buffer.pop_front();
  }

  void FlushFinalizedPuts() { while (buffer.size() && TryFinalizePut()); }

public:

  DequeRputDuct(
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

  ~DequeRputDuct() {
    FlushFinalizedPuts();
    while (buffer.size()) CancelPendingPut();
  }

  /**
   * TODO.
   *
   * @param val TODO.
   */
  bool TryPut(const T& val) {
    buffer.emplace_back(
      packet_t(val, ++epoch),
      uitsl::Request{}
    );
    emp_assert( uitsl::test_null( std::get<uitsl::Request>(buffer.back()) ) );
    PostPut();
    return true;
  }

  /**
   * TODO.
   *
   */
  bool TryFlush() const { return true; }

  [[noreturn]] size_t TryConsumeGets(size_t) const {
    emp_always_assert(false, "ConsumeGets called on DequeRputDuct");
    __builtin_unreachable();
  }

  [[noreturn]] const T& Get() const {
    emp_always_assert(false, "Get called on DequeRputDuct");
    __builtin_unreachable();
  }

  [[noreturn]] T& Get() {
    emp_always_assert(false, "Get called on DequeRputDuct");
    __builtin_unreachable();
  }

  static std::string GetType() { return "DequeRputDuct"; }

  std::string ToString() const {
    std::stringstream ss;
    ss << GetType() << '\n';
    ss << uitsl::format_member("this", static_cast<const void *>(this)) << '\n';
    ss << uitsl::format_member("InterProcAddress address", address) << '\n';
    return ss.str();
  }

};

} // namespace t
} // namespace uit

#endif // #ifndef UIT_DUCTS_PROC_IMPL_INLET_PUT_GROWING_TYPE_TRIVIAL_T__DEQUERPUTDUCT_HPP_INCLUDE
