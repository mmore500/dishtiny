#pragma once
#ifndef UIT_DUCTS_PROC_IMPL_INLET_ACCUMULATING_TYPE_TRIVIAL_IMPL_ACCUMULATINGTRIVIALISENDDUCT_HPP_INCLUDE
#define UIT_DUCTS_PROC_IMPL_INLET_ACCUMULATING_TYPE_TRIVIAL_IMPL_ACCUMULATINGTRIVIALISENDDUCT_HPP_INCLUDE

#include <algorithm>
#include <array>
#include <memory>
#include <stddef.h>
#include <string>

#include <mpi.h>

#include "../../../../../../../../third-party/Empirical/include/emp/base/always_assert.hpp"
#include "../../../../../../../../third-party/Empirical/include/emp/base/assert.hpp"
#include "../../../../../../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "../../../../../../../uitsl/datastructs/RingBuffer.hpp"
#include "../../../../../../../uitsl/debug/err_audit.hpp"
#include "../../../../../../../uitsl/distributed/MsgAccumulatorPacket.hpp"
#include "../../../../../../../uitsl/meta/t::static_test.hpp"
#include "../../../../../../../uitsl/mpi/audited_routines.hpp"
#include "../../../../../../../uitsl/mpi/mpi_utils.hpp"
#include "../../../../../../../uitsl/mpi/Request.hpp"
#include "../../../../../../../uitsl/nonce/CircularIndex.hpp"
#include "../../../../../../../uitsl/utility/print_utils.hpp"

#include "../../../../../../setup/InterProcAddress.hpp"

#include "../../../backend/MockBackEnd.hpp"

namespace uit {
namespace internal {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec, bool Buffered>
class AccumulatingTrivialIsendDuct {

public:

  using BackEndImpl = uit::MockBackEnd<ImplSpec>;

private:

  using T = typename ImplSpec::T;
  static_assert( uitsl::t::static_test<T>(), uitsl_t_message );

  using packet_t = uitsl::MsgAccumulatorPacket<T>;
  packet_t send_buffer{};
  packet_t pending_buffer{};
  uitsl::Request request;

  const uit::InterProcAddress address;

  void PostSendRequest() {

    emp_assert( uitsl::test_null( request ) );
    UITSL_Isend(
      &send_buffer,
      sizeof(packet_t),
      MPI_BYTE,
      address.GetOutletProc(),
      address.GetTag(),
      address.GetComm(),
      &request
    );
    emp_assert( !uitsl::test_null( request ) );

  }

  bool TryFinalizeSend() {
    emp_assert( !uitsl::test_null( request ) );
    return uitsl::test_completion( request );
  }

  void CancelPendingSend() {
    emp_assert( !uitsl::test_null( request ) );

    UITSL_Cancel( &request );
    UITSL_Request_free( &request );

    emp_assert( uitsl::test_null( request ) );

  }

public:

  AccumulatingTrivialIsendDuct(
    const uit::InterProcAddress& address_,
    std::shared_ptr<BackEndImpl> back_end
  ) : address(address_)
  { ; }

  ~AccumulatingTrivialIsendDuct() {
    if ( !uitsl::test_null( request ) ) TryFinalizeSend();
    if ( !uitsl::test_null( request ) ) CancelPendingSend();
  }

  /**
   * TODO.
   *
   * @param val TODO.
   */
  bool TryPut(const T& val) {
    pending_buffer.GetData() += val;
    pending_buffer.BumpEpoch();

    if constexpr (!Buffered) TryFlush();

    return true;

  }

  /**
   * TODO.
   */
  bool TryFlush() {

    if ( !uitsl::test_null( request ) ) TryFinalizeSend();

    if ( pending_buffer.GetData() == T{} ) return true;
    else if ( uitsl::test_null( request ) ) {
      send_buffer = std::exchange(pending_buffer, packet_t{});
      PostSendRequest();
      return true;
    } else return false;

  }

  [[noreturn]] size_t TryConsumeGets(size_t) const {
    emp_always_assert(
      false, "ConsumeGets called on AccumulatingTrivialIsendDuct"
    );
    __builtin_unreachable();
  }

  [[noreturn]] const T& Get() const {
    emp_always_assert(false, "Get called on AccumulatingTrivialIsendDuct");
    __builtin_unreachable();
  }

  [[noreturn]] T& Get() {
    emp_always_assert(false, "Get called on AccumulatingTrivialIsendDuct");
    __builtin_unreachable();
  }

  static std::string GetType() { return "AccumulatingTrivialIsendDuct"; }

  std::string ToString() const {
    std::stringstream ss;
    ss << GetType() << '\n';
    ss << uitsl::format_member("this", static_cast<const void *>(this)) << '\n';
    ss << uitsl::format_member("InterProcAddress address", address) << '\n';
    return ss.str();
  }

};

} // namespace internal
} // namespace uit

#endif // #ifndef UIT_DUCTS_PROC_IMPL_INLET_ACCUMULATING_TYPE_TRIVIAL_IMPL_ACCUMULATINGTRIVIALISENDDUCT_HPP_INCLUDE
