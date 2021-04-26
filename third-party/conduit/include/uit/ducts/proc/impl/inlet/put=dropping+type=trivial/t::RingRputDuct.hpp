#pragma once
#ifndef UIT_DUCTS_PROC_IMPL_INLET_PUT_DROPPING_TYPE_TRIVIAL_T__RINGRPUTDUCT_HPP_INCLUDE
#define UIT_DUCTS_PROC_IMPL_INLET_PUT_DROPPING_TYPE_TRIVIAL_T__RINGRPUTDUCT_HPP_INCLUDE

#include <algorithm>
#include <array>
#include <stddef.h>

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
class RingRputDuct {

public:

  using BackEndImpl = uit::RdmaBackEnd<ImplSpec>;

private:

  using T = typename ImplSpec::T;
  static_assert( uitsl::t::static_test<T>(), uitsl_t_message );
  constexpr inline static size_t N{ImplSpec::N};
  using packet_t = uitsl::RdmaPacket<T>;

  using buffer_t = emp::array<packet_t, N>;
  buffer_t buffer{};

  size_t epoch{};

  using index_t = uitsl::CircularIndex<N>;
  index_t put_position{};

  emp::array<uitsl::Request, N> put_requests;
  size_t pending_puts{};

  const uit::InterProcAddress address;

  std::shared_ptr<BackEndImpl> back_end;

  uitsl::Request target_offset_request;
  int target_offset;

  /*
   * notes
   *
   * key:
   * - R = open put request
   * - N = put position (where next put request will go)
   * - S = stalest request position
   * initial state:
   * ```
   * : S :
   * : N :
   * |   |   |   |   |   |   |   |   |   |   |
   * | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | N
   * ```
   *
   * after one put posted:
   * ```
   * : S : N :
   * | R |   |   |   |   |   |   |   |   |   |
   * | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | N
   * ```
   *
   * after another put posted:
   * ```
   * : S :   : N :
   * | R | R |   |   |   |   |   |   |   |   |
   * | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | N
   * ```
   *
   * after a put finalized:
   * ```
   *     : S : N :
   * |   | R |   |   |   |   |   |   |   |   |
   * | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | N
   * ```
   *
   * possible almost-full buffer arrangement:
   * ```
   * : S :                               : N :
   * | R | R | R | R | R | R | R | R | R |   |
   * | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | N
   * ```
   *
   * one more put posted:
   * ```
   * : N :
   * : S :
   * | R | R | R | R | R | R | R | R | R | R |
   * | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | N
   * ```
   *
   * one put finalized:
   * ```
   * : N : S :
   * |   | R | R | R | R | R | R | R | R | R |
   * | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | N
   * ```
   *
   * another put finalized:
   * ```
   * : N :   : S :
   * |   |   | R | R | R | R | R | R | R | R |
   * | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | N
   * ```
   */

  void PostPut() {

    // make sure that target offset has been received
    emp_assert( uitsl::test_completion(target_offset_request) );

    // TODO handle more than one at a time
    emp_assert( uitsl::test_null(put_requests[put_position]) );

    // TODO FIXME what kind of lock is needed here?
    back_end->GetWindowManager().LockShared( address.GetOutletProc() );

    back_end->GetWindowManager().Rput(
      address.GetOutletProc(),
      reinterpret_cast<const std::byte*>( &buffer[put_position] ),
      sizeof(packet_t),
      target_offset,
      &put_requests[put_position]
    );

    back_end->GetWindowManager().Unlock( address.GetOutletProc() );

    emp_assert( !uitsl::test_null(put_requests[put_position]) );

    ++put_position;
    ++pending_puts;

  }

  index_t CalcStalestPutPos() const { return put_position - pending_puts; }

  bool TryFinalizePut() {
    emp_assert( !uitsl::test_null(put_requests[CalcStalestPutPos()]) );

    if (uitsl::test_completion( put_requests[CalcStalestPutPos()] )) {
      --pending_puts;
      emp_assert( uitsl::test_null(put_requests[CalcStalestPutPos() - 1]) );
      return true;
    } else return false;

  }

  void CancelPendingPut() {
    emp_assert(!uitsl::test_null( put_requests[CalcStalestPutPos()] ));

    UITSL_Cancel( &put_requests[CalcStalestPutPos()] );
    UITSL_Request_free( &put_requests[CalcStalestPutPos()] );

    emp_assert(uitsl::test_null( put_requests[CalcStalestPutPos()] ));

    --pending_puts;
  }

  void FlushFinalizedPuts() { while (pending_puts && TryFinalizePut()); }

  /**
   * TODO.
   *
   * @param val TODO.
   */
  void DoPut(const T& val) {
    emp_assert( pending_puts < N );
    emp_assert( uitsl::test_null( put_requests[put_position] ) );
    buffer[put_position] = packet_t(val, ++epoch);
    PostPut();
    emp_assert( pending_puts <= N );
  }

  /**
   * TODO.
   *
   * @return TODO.
   */
  bool IsReadyForPut() {
    FlushFinalizedPuts();
    return pending_puts < N;
  }

public:

  RingRputDuct(
    const uit::InterProcAddress& address_,
    std::shared_ptr<BackEndImpl> back_end_
  ) : address(address_)
  , back_end(back_end_)
  {

    emp_assert( std::all_of(
      std::begin(put_requests),
      std::end(put_requests),
      [](const auto& req){ return uitsl::test_null( req ); }
    ) );

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

  ~RingRputDuct() {
    FlushFinalizedPuts();
    while (pending_puts) CancelPendingPut();
    emp_assert( std::all_of(
      std::begin(put_requests),
      std::end(put_requests),
      [](const auto& req){ return uitsl::test_null( req ); }
    ) );
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
   */
  bool TryFlush() const { return true; }

  [[noreturn]] size_t TryConsumeGets(size_t) const {
    emp_always_assert(false, "ConsumeGets called on RingRputDuct");
    __builtin_unreachable();
  }

  [[noreturn]] const T& Get() const {
    emp_always_assert(false, "Get called on RingRputDuct");
    __builtin_unreachable();
  }

  [[noreturn]] T& Get() {
    emp_always_assert(false, "Get called on RingRputDuct");
    __builtin_unreachable();
  }

  static std::string GetType() { return "RingRputDuct"; }

  std::string ToString() const {
    std::stringstream ss;
    ss << GetType() << '\n';
    ss << uitsl::format_member("this", static_cast<const void *>(this)) << '\n';
    ss << uitsl::format_member("buffer_t buffer", buffer[0]) << '\n';
    ss << uitsl::format_member("size_t pending_puts", pending_puts) << '\n';
    ss << uitsl::format_member("InterProcAddress address", address) << '\n';
    ss << uitsl::format_member("size_t put_position", put_position);
    return ss.str();
  }

};

} // namespace t
} // namespace uit

#endif // #ifndef UIT_DUCTS_PROC_IMPL_INLET_PUT_DROPPING_TYPE_TRIVIAL_T__RINGRPUTDUCT_HPP_INCLUDE
