#pragma once
#ifndef UIT_DUCTS_PROC_IMPL_INLET_PUT_DROPPING_TYPE_SPAN_IMPL_SPANRINGIMMEDIATESENDDUCT_HPP_INCLUDE
#define UIT_DUCTS_PROC_IMPL_INLET_PUT_DROPPING_TYPE_SPAN_IMPL_SPANRINGIMMEDIATESENDDUCT_HPP_INCLUDE

#include <algorithm>
#include <array>
#include <memory>
#include <stddef.h>
#include <string>

#include <mpi.h>

#include "../../../../../../../../third-party/cereal/include/cereal/archives/binary.hpp"
#include "../../../../../../../../third-party/Empirical/include/emp/base/always_assert.hpp"
#include "../../../../../../../../third-party/Empirical/include/emp/base/assert.hpp"
#include "../../../../../../../../third-party/Empirical/include/emp/base/optional.hpp"
#include "../../../../../../../../third-party/Empirical/include/emp/io/ContiguousStream.hpp"
#include "../../../../../../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "../../../../../../../uitsl/datastructs/RingBuffer.hpp"
#include "../../../../../../../uitsl/debug/err_audit.hpp"
#include "../../../../../../../uitsl/meta/s::static_test.hpp"
#include "../../../../../../../uitsl/mpi/mpi_utils.hpp"
#include "../../../../../../../uitsl/mpi/Request.hpp"
#include "../../../../../../../uitsl/nonce/CircularIndex.hpp"
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
template<typename ImmediateSendFunctor, typename ImplSpec>
class SpanRingImmediateSendDuct {

public:

  using BackEndImpl = uit::RuntimeSizeBackEnd<ImplSpec>;

private:

  using T = typename ImplSpec::T;
  static_assert( uitsl::s::static_test<T>(), uitsl_s_message );
  constexpr inline static size_t N{ImplSpec::N};

  using buffer_t = uitsl::RingBuffer< std::tuple<T, uitsl::Request>, N >;
  buffer_t buffer{};

  const uit::InterProcAddress address;

  emp::optional<size_t> runtime_size;

  void PostSendRequest() {
    emp_assert( uitsl::test_null( std::get<uitsl::Request>( buffer.GetHead() ) ) );
    emp_assert(
      !runtime_size.has_value()
      || *runtime_size == std::get<T>( buffer.GetHead() ).size()
    );

    ImmediateSendFunctor{}(
      std::get<T>( buffer.GetHead() ).data(),
      std::get<T>( buffer.GetHead() ).size()
        * sizeof( typename T::value_type ),
      MPI_BYTE,
      address.GetOutletProc(),
      address.GetTag(),
      address.GetComm(),
      &std::get<uitsl::Request>( buffer.GetHead() )
    );

    emp_assert(!uitsl::test_null(std::get<uitsl::Request>( buffer.GetHead() )));
  }

  bool TryFinalizeSend() {
    emp_assert( !uitsl::test_null( std::get<uitsl::Request>( buffer.GetTail() ) ) );

    if (uitsl::test_completion( std::get<uitsl::Request>( buffer.GetTail() ) )) {
      emp_assert( uitsl::test_null( std::get<uitsl::Request>(buffer.GetTail()) ) );
      uitsl_err_audit(!   buffer.PopTail()   );
      return true;
    } else return false;
  }

  void CancelPendingSend() {
    emp_assert( !uitsl::test_null( std::get<uitsl::Request>( buffer.GetTail() ) ) );

    UITSL_Cancel( &std::get<uitsl::Request>( buffer.GetTail() ) );
    UITSL_Request_free( &std::get<uitsl::Request>( buffer.GetTail() ) );

    emp_assert( uitsl::test_null( std::get<uitsl::Request>( buffer.GetTail() ) ) );

    uitsl_err_audit(!   buffer.PopTail()   );
  }

  void FlushFinalizedSends() { while (buffer.GetSize() && TryFinalizeSend()); }

  /**
   * TODO.
   *
   * @param val TODO.
   */
  void DoPut(const T& val) {
    emp_assert( buffer.GetSize() < N );

    uitsl_err_audit(!   buffer.PushHead()   );

    std::get<T>( buffer.GetHead() ) = val;

    PostSendRequest();
  }

  /**
   * TODO.
   *
   * @param val TODO.
   */
  template<typename P>
  void DoPut(P&& val) {
    emp_assert( buffer.GetSize() < N );

    uitsl_err_audit(!   buffer.PushHead()   );

    std::get<T>( buffer.GetHead() ) = std::forward<P>(val);

    PostSendRequest();
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  bool IsReadyForPut() {
    FlushFinalizedSends();
    return buffer.GetSize() < N;
  }

public:

  SpanRingImmediateSendDuct(
    const uit::InterProcAddress& address_,
    std::shared_ptr<BackEndImpl> back_end,
    const uit::RuntimeSizeBackEnd<ImplSpec>& rts
      =uit::RuntimeSizeBackEnd<ImplSpec>{}
  ) : address(address_)
  , runtime_size( [&]() -> emp::optional<size_t> {
    if ( rts.HasSize() ) return {rts.GetSize()};
    else if ( back_end->HasSize() ) return {back_end->GetSize()};
    else return std::nullopt;
  }() )
  { ; }

  ~SpanRingImmediateSendDuct() {
    FlushFinalizedSends();
    while ( buffer.GetSize() ) CancelPendingSend();
  }

  /**
   * TODO.
   *
   * @param val TODO.
   */
  bool TryPut(const T& val) {
    if (IsReadyForPut()) { DoPut(val); return true; }
    else return false;
  }

  /**
   * TODO.
   *
   * @param val TODO.
   */
  template<typename P>
  bool TryPut(P&& val) {
    if (IsReadyForPut()) { DoPut(std::forward<P>(val)); return true; }
    else return false;
  }

  /**
   * TODO.
   */
  bool TryFlush() const { return true; }

  [[noreturn]] size_t TryConsumeGets(size_t) const {
    emp_always_assert(false, "ConsumeGets called on SpanRingImmediateSendDuct");
    __builtin_unreachable();
  }

  [[noreturn]] const T& Get() const {
    emp_always_assert(false, "Get called on SpanRingImmediateSendDuct");
    __builtin_unreachable();
  }

  [[noreturn]] T& Get() {
    emp_always_assert(false, "Get called on SpanRingImmediateSendDuct");
    __builtin_unreachable();
  }

  static std::string GetType() { return "SpanRingImmediateSendDuct"; }

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

#endif // #ifndef UIT_DUCTS_PROC_IMPL_INLET_PUT_DROPPING_TYPE_SPAN_IMPL_SPANRINGIMMEDIATESENDDUCT_HPP_INCLUDE
