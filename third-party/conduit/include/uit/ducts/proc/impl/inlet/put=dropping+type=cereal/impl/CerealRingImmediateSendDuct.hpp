#pragma once
#ifndef UIT_DUCTS_PROC_IMPL_INLET_PUT_DROPPING_TYPE_CEREAL_IMPL_CEREALRINGIMMEDIATESENDDUCT_HPP_INCLUDE
#define UIT_DUCTS_PROC_IMPL_INLET_PUT_DROPPING_TYPE_CEREAL_IMPL_CEREALRINGIMMEDIATESENDDUCT_HPP_INCLUDE

#include <algorithm>
#include <array>
#include <memory>
#include <stddef.h>
#include <string>

#include <mpi.h>

#include "../../../../../../../../third-party/cereal/include/cereal/archives/binary.hpp"
#include "../../../../../../../../third-party/Empirical/include/emp/base/always_assert.hpp"
#include "../../../../../../../../third-party/Empirical/include/emp/base/assert.hpp"
#include "../../../../../../../../third-party/Empirical/include/emp/io/ContiguousStream.hpp"
#include "../../../../../../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "../../../../../../../uitsl/datastructs/RingBuffer.hpp"
#include "../../../../../../../uitsl/debug/err_audit.hpp"
#include "../../../../../../../uitsl/meta/c::static_test.hpp"
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
template<typename ImmediateSendFunctor, typename ImplSpec>
class CerealRingImmediateSendDuct {

public:

  using BackEndImpl = uit::MockBackEnd<ImplSpec>;

private:

  using T = typename ImplSpec::T;
  static_assert( uitsl::c::static_test<T>(), uitsl_c_message );
  constexpr inline static size_t N{ImplSpec::N};

  using buffer_t = uitsl::RingBuffer<
    std::tuple<emp::ContiguousStream, uitsl::Request>,
    N
  >;
  buffer_t buffer{};

  const uit::InterProcAddress address;

  void PostSendRequest() {

    emp_assert( uitsl::test_null( std::get<uitsl::Request>( buffer.GetHead() ) ) );
    ImmediateSendFunctor{}(
      std::get<emp::ContiguousStream>( buffer.GetHead() ).GetData(),
      std::get<emp::ContiguousStream>( buffer.GetHead() ).GetSize(),
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

    { // oarchive flushes on destruction
      std::get<emp::ContiguousStream>( buffer.GetHead() ).Reset();
      cereal::BinaryOutputArchive oarchive(
        std::get<emp::ContiguousStream>( buffer.GetHead() )
      );
      oarchive(val);
    }
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

  CerealRingImmediateSendDuct(
    const uit::InterProcAddress& address_,
    std::shared_ptr<BackEndImpl> back_end
  ) : address(address_)
  { ; }

  ~CerealRingImmediateSendDuct() {
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
   */
  bool TryFlush() const { return true; }

  [[noreturn]] size_t TryConsumeGets(size_t) const {
    emp_always_assert(
      false, "ConsumeGets called on CerealRingImmediateSendDuct"
    );
    __builtin_unreachable();
  }

  [[noreturn]] const T& Get() const {
    emp_always_assert(false, "Get called on CerealRingImmediateSendDuct");
    __builtin_unreachable();
  }

  [[noreturn]] T& Get() {
    emp_always_assert(false, "Get called on CerealRingImmediateSendDuct");
    __builtin_unreachable();
  }

  static std::string GetType() { return "CerealRingImmediateSendDuct"; }

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

#endif // #ifndef UIT_DUCTS_PROC_IMPL_INLET_PUT_DROPPING_TYPE_CEREAL_IMPL_CEREALRINGIMMEDIATESENDDUCT_HPP_INCLUDE
