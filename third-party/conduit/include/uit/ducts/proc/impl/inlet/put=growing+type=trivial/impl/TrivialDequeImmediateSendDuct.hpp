#pragma once
#ifndef UIT_DUCTS_PROC_IMPL_INLET_PUT_GROWING_TYPE_TRIVIAL_IMPL_TRIVIALDEQUEIMMEDIATESENDDUCT_HPP_INCLUDE
#define UIT_DUCTS_PROC_IMPL_INLET_PUT_GROWING_TYPE_TRIVIAL_IMPL_TRIVIALDEQUEIMMEDIATESENDDUCT_HPP_INCLUDE

#include <algorithm>
#include <memory>
#include <stddef.h>
#include <string>
#include <tuple>
#include <utility>

#include <mpi.h>

#include "../../../../../../../../third-party/Empirical/include/emp/base/always_assert.hpp"
#include "../../../../../../../../third-party/Empirical/include/emp/base/assert.hpp"
#include "../../../../../../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "../../../../../../../uitsl/meta/t::static_test.hpp"
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
class TrivialDequeImmediateSendDuct {

public:

  using BackEndImpl = uit::MockBackEnd<ImplSpec>;

private:

  using T = typename ImplSpec::T;
  static_assert( uitsl::t::static_test<T>(), uitsl_t_message );

  // newest requests are pushed back, oldest requests are at front
  std::deque<std::tuple<T, uitsl::Request>> buffer;

  const uit::InterProcAddress address;

  void PostSend() {
    emp_assert( uitsl::test_null(
      std::get<uitsl::Request>(buffer.back())
    ) );

    ImmediateSendFunctor{}(
      &std::get<T>(buffer.back()),
      sizeof(T),
      MPI_BYTE,
      address.GetOutletProc(),
      address.GetTag(),
      address.GetComm(),
      &std::get<uitsl::Request>(buffer.back())
    );

  }

  bool TryFinalizeSend() {
    emp_assert( !uitsl::test_null( std::get<uitsl::Request>(buffer.front()) ) );

    if (uitsl::test_completion( std::get<uitsl::Request>(buffer.front()) )) {
      emp_assert( uitsl::test_null( std::get<uitsl::Request>(buffer.front()) ) );
      buffer.pop_front();
      return true;
    } else return false;

  }

  void CancelPendingSend() {
    emp_assert( !uitsl::test_null( std::get<uitsl::Request>(buffer.front()) ) );

    UITSL_Cancel( &std::get<uitsl::Request>(buffer.front()) );
    UITSL_Request_free( &std::get<uitsl::Request>(buffer.front()) );

    emp_assert( uitsl::test_null( std::get<uitsl::Request>(buffer.front()) ) );

    buffer.pop_front();
  }

  void FlushFinalizedSends() { while (buffer.size() && TryFinalizeSend()); }

public:

  TrivialDequeImmediateSendDuct(
    const uit::InterProcAddress& address_,
    std::shared_ptr<BackEndImpl> back_end
  ) : address(address_)
  { }

  ~TrivialDequeImmediateSendDuct() {
    FlushFinalizedSends();
    while (buffer.size()) CancelPendingSend();
  }

  /**
   * TODO.
   *
   * @param val TODO.
   */
  bool TryPut(const T& val) {
    buffer.emplace_back(
      val,
      uitsl::Request{}
    );
    emp_assert( uitsl::test_null( std::get<uitsl::Request>(buffer.back()) ) );
    PostSend();
    return true;
  }

  /**
   * TODO.
   *
   */
  bool TryFlush() const { return true; }

  [[noreturn]] size_t TryConsumeGets(size_t) const {
    emp_always_assert(
      false, "ConsumeGets called on TrivialDequeImmediateSendDuct"
    );
    __builtin_unreachable();
  }

  [[noreturn]] const T& Get() const {
    emp_always_assert(false, "Get called on TrivialDequeImmediateSendDuct");
    __builtin_unreachable();
  }

  [[noreturn]] T& Get() {
    emp_always_assert(false, "Get called on TrivialDequeImmediateSendDuct");
    __builtin_unreachable();
  }

  static std::string GetType() { return "TrivialDequeImmediateSendDuct"; }

  std::string ToString() const {
    std::stringstream ss;
    ss << GetType() << '\n';
    ss << uitsl::format_member("this", static_cast<const void *>(this)) << '\n';
    ss << uitsl::format_member("buffer_t buffer", buffer[0]) << '\n';
    ss << uitsl::format_member("InterProcAddress address", address) << '\n';
    return ss.str();
  }

};

} // namespace internal
} // namespace uit

#endif // #ifndef UIT_DUCTS_PROC_IMPL_INLET_PUT_GROWING_TYPE_TRIVIAL_IMPL_TRIVIALDEQUEIMMEDIATESENDDUCT_HPP_INCLUDE
