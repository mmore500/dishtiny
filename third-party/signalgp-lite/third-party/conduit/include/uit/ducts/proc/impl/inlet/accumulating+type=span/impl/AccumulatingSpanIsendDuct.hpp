#pragma once
#ifndef UIT_DUCTS_PROC_IMPL_INLET_ACCUMULATING_TYPE_SPAN_IMPL_ACCUMULATINGSPANISENDDUCT_HPP_INCLUDE
#define UIT_DUCTS_PROC_IMPL_INLET_ACCUMULATING_TYPE_SPAN_IMPL_ACCUMULATINGSPANISENDDUCT_HPP_INCLUDE

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
#include "../../../../../../../uitsl/distributed/MsgAccumulatorBundle.hpp"
#include "../../../../../../../uitsl/meta/s::static_test.hpp"
#include "../../../../../../../uitsl/mpi/audited_routines.hpp"
#include "../../../../../../../uitsl/mpi/mpi_utils.hpp"
#include "../../../../../../../uitsl/mpi/Request.hpp"
#include "../../../../../../../uitsl/polyfill/identity.hpp"
#include "../../../../../../../uitsl/utility/print_utils.hpp"

#include "../../../../../../setup/InterProcAddress.hpp"

#include "../../../backend/RuntimeSizeBackEnd.hpp"

namespace uit {
namespace internal {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec, bool Buffered>
class AccumulatingSpanIsendDuct {

public:

  using BackEndImpl = uit::RuntimeSizeBackEnd<ImplSpec>;

private:

  const uit::InterProcAddress address;

  using T = typename ImplSpec::T;
  using value_type = typename T::value_type;
  static_assert( uitsl::s::static_test<T>(), uitsl_s_message );

  using bundle_t = uitsl::MsgAccumulatorBundle<value_type>;
  bundle_t send_buffer;
  bundle_t pending_buffer;
  uitsl::Request request;

  void PostSendRequest() {

    emp_assert( uitsl::test_null( request ) );
    UITSL_Isend(
      send_buffer.data(),
      send_buffer.byte_size(),
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

  AccumulatingSpanIsendDuct(
    const uit::InterProcAddress& address_,
    std::shared_ptr<BackEndImpl> back_end,
    const uit::RuntimeSizeBackEnd<ImplSpec>& rts
      =uit::RuntimeSizeBackEnd<ImplSpec>{}
  ) : address(address_)
  , send_buffer( rts.HasSize() ? rts.GetSize() : back_end->GetSize() )
  , pending_buffer( rts.HasSize() ? rts.GetSize() : back_end->GetSize() ) {
    emp_assert( rts.HasSize() || back_end->HasSize() );
  }

  ~AccumulatingSpanIsendDuct() {
    if ( !uitsl::test_null( request ) ) TryFinalizeSend();
    if ( !uitsl::test_null( request ) ) CancelPendingSend();
  }

  /**
   * TODO.
   *
   * @param val TODO.
   */
  bool TryPut(const T& val) {
    pending_buffer.BumpData(val);
    pending_buffer.BumpEpoch();

    if constexpr (!Buffered) TryFlush();

    return true;

  }

  /**
   * TODO.
   */
  bool TryFlush() {

    if ( !uitsl::test_null( request ) ) TryFinalizeSend();

    if ( uitsl::test_null( request ) ) {
      if ( std::any_of(
        std::begin(pending_buffer.GetData()),
        std::end(pending_buffer.GetData()),
        std::identity
      ) ) {
        std::swap( send_buffer, pending_buffer );
        pending_buffer.Reset();
        PostSendRequest();
      }
      return true;
    } else return false;

  }

  [[noreturn]] size_t TryConsumeGets(size_t) const {
    emp_always_assert(false, "ConsumeGets called on AccumulatingSpanIsendDuct");
    __builtin_unreachable();
  }

  [[noreturn]] const T& Get() const {
    emp_always_assert(false, "Get called on AccumulatingSpanIsendDuct");
    __builtin_unreachable();
  }

  [[noreturn]] T& Get() {
    emp_always_assert(false, "Get called on AccumulatingSpanIsendDuct");
    __builtin_unreachable();
  }

  static std::string GetType() { return "AccumulatingSpanIsendDuct"; }

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

#endif // #ifndef UIT_DUCTS_PROC_IMPL_INLET_ACCUMULATING_TYPE_SPAN_IMPL_ACCUMULATINGSPANISENDDUCT_HPP_INCLUDE
