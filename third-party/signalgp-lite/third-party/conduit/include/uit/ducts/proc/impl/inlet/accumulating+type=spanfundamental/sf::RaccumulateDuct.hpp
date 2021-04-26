#pragma once
#ifndef UIT_DUCTS_PROC_IMPL_INLET_ACCUMULATING_TYPE_SPANFUNDAMENTAL_SF__RACCUMULATEDUCT_HPP_INCLUDE
#define UIT_DUCTS_PROC_IMPL_INLET_ACCUMULATING_TYPE_SPANFUNDAMENTAL_SF__RACCUMULATEDUCT_HPP_INCLUDE

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
#include "../../../../../../uitsl/distributed/RdmaAccumulatorBundle.hpp"
#include "../../../../../../uitsl/distributed/RdmaWindowManager.hpp"
#include "../../../../../../uitsl/meta/f::static_test.hpp"
#include "../../../../../../uitsl/meta/s::static_test.hpp"
#include "../../../../../../uitsl/mpi/mpi_utils.hpp"
#include "../../../../../../uitsl/mpi/Request.hpp"
#include "../../../../../../uitsl/nonce/CircularIndex.hpp"
#include "../../../../../../uitsl/utility/print_utils.hpp"

#include "../../../../../setup/InterProcAddress.hpp"

#include "../../backend/RuntimeSizeBackEnd.hpp"
#include "../../backend/RuntimeSizeRdmaBackEnd.hpp"

namespace uit {
namespace sf {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
class RaccumulateDuct {

public:

  using BackEndImpl = uit::RuntimeSizeRdmaBackEnd<ImplSpec>;

private:

  const uit::InterProcAddress address;

  std::shared_ptr<BackEndImpl> back_end;

  using T = typename ImplSpec::T;
  using value_type = typename T::value_type;
  static_assert( uitsl::s::static_test<T>(), uitsl_s_message );
  static_assert(
    uitsl::f::static_test<value_type>(), uitsl_f_message
  );
  constexpr inline static size_t N{ImplSpec::N};
  using bundle_t = uitsl::RdmaAccumulatorBundle<value_type>;

  uitsl::Request request{};
  bundle_t prep_buffer;
  bundle_t send_buffer;

  uitsl::Request target_offset_request;
  int target_offset;

  void DoPostAccumulate() {

    emp_assert( uitsl::test_null( request ) );

    std::swap( send_buffer, prep_buffer );
    prep_buffer.Reset();

    // TODO FIXME what kind of lock is needed here?
    back_end->GetWindowManager().LockShared( address.GetOutletProc() );

    back_end->GetWindowManager().template Raccumulate<value_type>(
      address.GetOutletProc(),
      reinterpret_cast<std::byte*>( send_buffer.data() ),
      send_buffer.byte_size(),
      target_offset,
      &request
    );

    back_end->GetWindowManager().Unlock( address.GetOutletProc() );

    emp_assert( !uitsl::test_null( request ) );

  }

  void TryPostAccumulate() {
    if (uitsl::test_completion( request )) DoPostAccumulate();
  }

public:

  RaccumulateDuct(
    const uit::InterProcAddress& address_,
    std::shared_ptr<BackEndImpl> back_end_,
    const uit::RuntimeSizeBackEnd<ImplSpec>& rts
      =uit::RuntimeSizeBackEnd<ImplSpec>{}
  ) : address(address_)
  , back_end(back_end_)
  , prep_buffer( rts.HasSize() ? rts.GetSize() : back_end->GetSize() )
  , send_buffer( rts.HasSize() ? rts.GetSize() : back_end->GetSize() ) {

    emp_assert( rts.HasSize() || back_end->HasSize() );

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
    prep_buffer.BumpData( val );
    prep_buffer.BumpEpoch();
    TryPostAccumulate();
    return true;
  }

  /**
   * TODO.
   *
   */
  bool TryFlush() const { return true; }

  [[noreturn]] size_t TryConsumeGets(size_t) const {
    emp_always_assert(false, "ConsumeGets called on RaccumulateDuct");
    __builtin_unreachable();
  }

  [[noreturn]] const T& Get() const {
    emp_always_assert(false, "Get called on RaccumulateDuct");
    __builtin_unreachable();
  }

  [[noreturn]] T& Get() {
    emp_always_assert(false, "Get called on RaccumulateDuct");
    __builtin_unreachable();
  }

  static std::string GetType() { return "RaccumulateDuct"; }

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

#endif // #ifndef UIT_DUCTS_PROC_IMPL_INLET_ACCUMULATING_TYPE_SPANFUNDAMENTAL_SF__RACCUMULATEDUCT_HPP_INCLUDE
