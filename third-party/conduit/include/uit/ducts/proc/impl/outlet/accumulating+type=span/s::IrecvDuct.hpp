#pragma once
#ifndef UIT_DUCTS_PROC_IMPL_OUTLET_ACCUMULATING_TYPE_SPAN_S__IRECVDUCT_HPP_INCLUDE
#define UIT_DUCTS_PROC_IMPL_OUTLET_ACCUMULATING_TYPE_SPAN_S__IRECVDUCT_HPP_INCLUDE

#include <algorithm>
#include <array>
#include <stddef.h>

#include <mpi.h>

#include "../../../../../../../third-party/Empirical/include/emp/base/always_assert.hpp"
#include "../../../../../../../third-party/Empirical/include/emp/base/assert.hpp"
#include "../../../../../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "../../../../../../uitsl/debug/WarnOnce.hpp"
#include "../../../../../../uitsl/distributed/MsgAccumulatorBundle.hpp"
#include "../../../../../../uitsl/meta/s::static_test.hpp"
#include "../../../../../../uitsl/mpi/mpi_utils.hpp"
#include "../../../../../../uitsl/mpi/Request.hpp"
#include "../../../../../../uitsl/utility/print_utils.hpp"

#include "../../../../../setup/InterProcAddress.hpp"

#include "../../backend/RuntimeSizeBackEnd.hpp"

namespace uit {
namespace s {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
class IrecvDuct {

public:

  using BackEndImpl = uit::RuntimeSizeBackEnd<ImplSpec>;

private:

  const uit::InterProcAddress address;

  std::shared_ptr<BackEndImpl> back_end;

  using T = typename ImplSpec::T;
  static_assert( uitsl::s::static_test<T>(), uitsl_s_message );
  using value_type = typename T::value_type;

  using bundle_t = uitsl::MsgAccumulatorBundle<value_type>;
  bundle_t buffer;
  bundle_t cache;

  uitsl::Request receive_request;

  void PostReceiveRequest() {
    emp_assert( uitsl::test_null( receive_request ) );
    UITSL_Irecv(
      buffer.data(),
      buffer.byte_size(),
      MPI_BYTE,
      address.GetInletProc(),
      address.GetTag(),
      address.GetComm(),
      &receive_request
    );
    emp_assert( !uitsl::test_null( receive_request ) );
  }

  void CancelReceiveRequest() {
    emp_assert( !uitsl::test_null( receive_request ) );

    UITSL_Cancel( &receive_request );
    UITSL_Request_free( &receive_request );

    emp_assert( uitsl::test_null( receive_request ) );
  }

  // returns true if receive was full
  bool TryReceive() {

    emp_assert( !uitsl::test_null( receive_request ) );

    const bool res = uitsl::test_completion( receive_request );

    if (res) {
      cache += buffer;
      PostReceiveRequest();
    }

    return res;

  }

  void FlushReceives() { while ( TryReceive() ); }

public:

  IrecvDuct(
    const uit::InterProcAddress& address_,
    std::shared_ptr<BackEndImpl> back_end_,
    const BackEndImpl& rts = BackEndImpl{}
  ) : address(address_)
  , back_end(back_end_)
  , buffer( rts.HasSize() ? rts.GetSize() : back_end->GetSize() )
  , cache( rts.HasSize() ? rts.GetSize() : back_end->GetSize() )
  {
    emp_assert( rts.HasSize() || back_end->HasSize() );
    PostReceiveRequest();
  }

  ~IrecvDuct() {
    FlushReceives();
    CancelReceiveRequest();
    emp_assert( uitsl::test_null( receive_request ) );
  }

  [[noreturn]] bool TryPut(const T&) const {
    emp_always_assert(false, "TryPut called on IrecvDuct");
    __builtin_unreachable();
  }

  [[noreturn]] bool TryFlush() const {
    emp_always_assert(false, "Flush called on IrecvDuct");
    __builtin_unreachable();
  }

  /**
   * TODO.
   *
   * @param num_requested TODO.
   * @return number items consumed.
   */
  size_t TryConsumeGets(const size_t num_requested) {

    emp_assert( num_requested == std::numeric_limits<size_t>::max() );

    cache.Reset();

    FlushReceives();

    return cache.GetEpoch();

  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  const T& Get() const { return cache.GetData(); }

  /**
   * TODO.
   *
   * @return TODO.
   */
  T& Get() { return cache.GetData(); }

  static std::string GetName() { return "IrecvDuct"; }

  static constexpr bool CanStep() { return false; }

  std::string ToString() const {
    std::stringstream ss;
    ss << GetName() << '\n';
    ss << uitsl::format_member("this", static_cast<const void *>(this)) << '\n';
    ss << uitsl::format_member("InterProcAddress address", address) << '\n';
    return ss.str();
  }

};

} // namespace s
} // namespace uit

#endif // #ifndef UIT_DUCTS_PROC_IMPL_OUTLET_ACCUMULATING_TYPE_SPAN_S__IRECVDUCT_HPP_INCLUDE
