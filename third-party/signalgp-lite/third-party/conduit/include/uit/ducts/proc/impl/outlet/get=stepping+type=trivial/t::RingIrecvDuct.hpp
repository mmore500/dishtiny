#pragma once
#ifndef UIT_DUCTS_PROC_IMPL_OUTLET_GET_STEPPING_TYPE_TRIVIAL_T__RINGIRECVDUCT_HPP_INCLUDE
#define UIT_DUCTS_PROC_IMPL_OUTLET_GET_STEPPING_TYPE_TRIVIAL_T__RINGIRECVDUCT_HPP_INCLUDE

#include <algorithm>
#include <array>
#include <memory>
#include <stddef.h>

#include <mpi.h>

#include "../../../../../../../third-party/Empirical/include/emp/base/always_assert.hpp"
#include "../../../../../../../third-party/Empirical/include/emp/base/assert.hpp"
#include "../../../../../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "../../../../../../uitsl/datastructs/RingBuffer.hpp"
#include "../../../../../../uitsl/datastructs/SiftingArray.hpp"
#include "../../../../../../uitsl/debug/err_audit.hpp"
#include "../../../../../../uitsl/meta/t::static_test.hpp"
#include "../../../../../../uitsl/mpi/mpi_utils.hpp"
#include "../../../../../../uitsl/utility/print_utils.hpp"

#include "../../../../../setup/InterProcAddress.hpp"

#include "../../backend/MockBackEnd.hpp"

namespace uit {
namespace t {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
class RingIrecvDuct {

public:

  using BackEndImpl = uit::MockBackEnd<ImplSpec>;

private:

  using T = typename ImplSpec::T;
  static_assert( uitsl::t::static_test<T>(), uitsl_t_message );
  constexpr inline static size_t N{ImplSpec::N};

  // one extra in the data buffer to hold the current get
  uitsl::RingBuffer<T, N + 1> data;
  uitsl::SiftingArray<MPI_Request, N> requests;

  const uit::InterProcAddress address;

  void PostReceiveRequest() {
    uitsl_err_audit(!
      data.PushHead()
    );
    requests.PushBack( MPI_REQUEST_NULL );

    emp_assert( uitsl::test_null( requests.Back() ) );
    UITSL_Irecv(
      &data.GetHead(),
      sizeof(T),
      MPI_BYTE,
      address.GetInletProc(),
      address.GetTag(),
      address.GetComm(),
      &requests.Back()
    );
    emp_assert( !uitsl::test_null( requests.Back() ) );

  }

  void CancelReceiveRequest() {
    emp_assert( !uitsl::test_null( requests.Back() ) );

    UITSL_Cancel(  &requests.Back() );
    UITSL_Request_free( &requests.Back() );

    emp_assert( uitsl::test_null( requests.Back() ) );

    uitsl_err_audit(!  data.PopTail()  );
    uitsl_err_audit(!  requests.PopBack()  );

  }

  void CancelOpenReceiveRequests() {
    while ( requests.GetSize() ) CancelReceiveRequest();
  }

  void TestRequests() {

    // MPICH Testsome returns negative outcount for zero count calls
    // so let's boogie out early to avoid drama
    if (requests.GetSize() == 0) return;

    emp_assert( std::none_of(
      std::begin(requests),
      std::end(requests),
      [](const auto& req){ return uitsl::test_null( req ); }
    ) );

    thread_local emp::array<int, N> out_indices; // ignored
    int num_received; // ignored

    UITSL_Testsome(
      requests.GetSize(), // int count
      requests.GetData(), // MPI_Request array_of_requests[]
      &num_received, // int *outcount
      out_indices.data(), // int *indices
      MPI_STATUSES_IGNORE // MPI_Status array_of_statuses[]
    );

    emp_assert( num_received >= 0 );
    emp_assert( static_cast<size_t>(num_received) <= requests.GetSize() );

  }

  void TryFulfillReceiveRequests() {
    TestRequests();
    requests.SiftByValue(
      [](const auto& request){ return request != MPI_REQUEST_NULL; }
    );
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  size_t CountUnconsumedGets() {
    TryFulfillReceiveRequests();
    return data.GetSize() - requests.GetSize() - 1;
  }

public:

  RingIrecvDuct(
    const uit::InterProcAddress& address_,
    std::shared_ptr<BackEndImpl> back_end
  ) : address(address_) {

    data.PushHead( T{} ); // value-initialized initial Get item
    for (size_t i = 0; i < N; ++i) PostReceiveRequest();
    emp_assert( std::none_of(
      std::begin(requests),
      std::end(requests),
      [](const auto& req){ return uitsl::test_null( req ); }
    ) );
  }

  ~RingIrecvDuct() {
    while ( CountUnconsumedGets() ) TryConsumeGets( CountUnconsumedGets() );
    CancelOpenReceiveRequests();
  }

  [[noreturn]] bool TryPut(const T&) const {
    emp_always_assert(false, "TryPut called on RingIrecvDuct");
    __builtin_unreachable();
  }

  /**
   * TODO.
   *
   */
  [[noreturn]] bool TryFlush() const {
    emp_always_assert(false, "Flush called on RingIrecvDuct");
    __builtin_unreachable();
  }

  /**
   * TODO.
   *
   * @param num_requested TODO.
   * @return number items consumed.
   */
  size_t TryConsumeGets(const size_t num_requested) {

    size_t requested_countdown{ num_requested };
    size_t batch_countdown{ CountUnconsumedGets() };
    bool full_batch = (batch_countdown == N);

    while ( batch_countdown && requested_countdown ) {

      --batch_countdown;
      --requested_countdown;
      uitsl_err_audit(!   data.PopTail()   );
      PostReceiveRequest();

      if (full_batch && batch_countdown == 0) {
        batch_countdown = CountUnconsumedGets();
        full_batch = (batch_countdown == N);
      }
    }

    const size_t num_consumed = num_requested - requested_countdown;
    return num_consumed;
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  const T& Get() const { return data.GetTail(); }

  /**
   * TODO.
   *
   * @return TODO.
   */
  T& Get() { return data.GetTail(); }

  static std::string GetName() { return "RingIrecvDuct"; }

  static constexpr bool CanStep() { return true; }

  std::string ToString() const {
    std::stringstream ss;
    ss << GetName() << '\n';
    ss << uitsl::format_member("this", static_cast<const void *>(this)) << '\n';
    ss << uitsl::format_member("InterProcAddress address", address) << '\n';
    return ss.str();
  }

};

} // namespace t
} // namespace uit

#endif // #ifndef UIT_DUCTS_PROC_IMPL_OUTLET_GET_STEPPING_TYPE_TRIVIAL_T__RINGIRECVDUCT_HPP_INCLUDE
