#pragma once
#ifndef UIT_DUCTS_PROC_IMPL_BACKEND_IMPL_OUTLETMEMORYAGGREGATOR_HPP_INCLUDE
#define UIT_DUCTS_PROC_IMPL_BACKEND_IMPL_OUTLETMEMORYAGGREGATOR_HPP_INCLUDE

#include <algorithm>

#include "../../../../../../../third-party/Empirical/include/emp/base/assert.hpp"
#include "../../../../../../../third-party/Empirical/include/emp/base/optional.hpp"
#include "../../../../../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "../../../../../../uitsl/algorithm/upper_uniquify.hpp"

#include "../../../../../fixtures/Source.hpp"
#include "../../../../../setup/InterProcAddress.hpp"
#include "../../../../../spouts/Outlet.hpp"

namespace uit {

template<typename AggregatorSpec>
class OutletMemoryAggregator {

  using address_t = uit::InterProcAddress;
  emp::vector<address_t> addresses;

  template<typename T>
  using outlet_wrapper_t = typename AggregatorSpec::template outlet_wrapper_t<
    T
  >;
  emp::optional<outlet_wrapper_t<uit::Outlet<AggregatorSpec>>> outlet;

  // multimap of index -> value_type
  using T = typename AggregatorSpec::T;
  T buffer{};

  // incremented every time TryConsumeGets is called with
  // requested == std::numeric_limits<size_t>::max()
  // then reset to zero once every member of the pool has called
  size_t jump_call_counter{};

  size_t current_request{}; // num jump steps requested in current round
  size_t current_num_consumed{}; // num jump steps realized in current round

  using value_type = typename AggregatorSpec::T::mapped_type;

  std::unordered_map<int, bool> hit_since_last_consume;
  bool dry_flag{ false };

  bool PreventRedundantDryConsumes(const int tag) {
    if ( !dry_flag ) return true;
    else if ( hit_since_last_consume.at(tag) ) {
      dry_flag = false;
      for (auto& [k, v] : hit_since_last_consume) v = false;
      return true;
    } else return false;
  }

  size_t DoTryStepGets(const size_t num_requested, const int tag) {

    size_t num_requested_countdown{ num_requested };

    do {

      emp_assert( buffer.count( tag ) );

      const auto lower_bound = buffer.lower_bound( tag );
      const size_t cur_step = std::min(
        num_requested_countdown,
        buffer.count( tag ) - 1
      );
      buffer.erase(
        lower_bound,
        std::next(lower_bound, cur_step)
      );
      num_requested_countdown -= cur_step;

    } while (
      num_requested_countdown
      && PreventRedundantDryConsumes( tag )
      && [this](){
        const bool res = outlet->TryStep( 1 );
        dry_flag = !res;
        return res;
      }()
      && [this](){
        buffer.merge( outlet->Get() );
        return true;
      }()
    );

    hit_since_last_consume[tag] = true;

    return num_requested - num_requested_countdown;

  }

  // TODO latest results are potentially lost if not in the latest buffer
  size_t DoTryJumpGets(const int tag) {

    const size_t buffer_steps = outlet->TryStep(
      std::numeric_limits<size_t>::max()
    );

    // estimate value steps
    const size_t approx_steps = (
      buffer_steps * outlet->Get().size() / GetSize()
    );

    buffer.merge( outlet->Get() );

    uitsl::upper_uniquify( buffer );

    return approx_steps;

  }

  void CheckCallingProc() const {
    [[maybe_unused]] const auto& rep = addresses.front();
    emp_assert( rep.GetOutletProc() == uitsl::get_rank( rep.GetComm() ) );
  }

public:

  bool IsInitialized() const { return outlet.has_value(); }

  size_t GetSize() const { return addresses.size(); }

  /// Register a duct for an entry in the pool.
  void Register(const address_t& address) {
    emp_assert( !IsInitialized() );
    emp_assert( std::find(
      std::begin( addresses ), std::end( addresses ), address
    ) == std::end(addresses) );
    addresses.push_back(address);
  }

  /// Get the querying duct's current value from the underlying duct.
  value_type& Get(const int tag) {
    emp_assert( IsInitialized() );
    CheckCallingProc();
    emp_assert( buffer.count(tag) );
    return buffer.lower_bound(tag)->second;
  }

  /// Get the querying duct's current value from the underlying duct.
  const value_type& Get(const int tag) const {
    emp_assert( IsInitialized() );
    CheckCallingProc();
    emp_assert( buffer.count(tag) );
    return buffer.lower_bound(tag)->second;
  }

  /// Every member of the pool should call this with same requested.
  size_t TryConsumeGets(
    const size_t requested,
    const int tag
  ) {
    emp_assert( IsInitialized() );
    CheckCallingProc();

    if ( requested == std::numeric_limits<size_t>::max()) {
      if ( jump_call_counter == 0 ) {
        current_num_consumed = DoTryJumpGets(tag);
      }

      ++jump_call_counter;
      jump_call_counter %= addresses.size();

      return current_num_consumed;
    } else {
      emp_assert( jump_call_counter == 0 );
      return DoTryStepGets(requested, tag);
    }

  }

  /// Call after all members have requested a position in the pool.
  void Initialize() {

    emp_assert( !IsInitialized() );

    emp_assert( std::adjacent_find(
      std::begin(addresses), std::end(addresses),
      [](const auto& a, const auto& b){
        return a.WhichProcsThreads() != b.WhichProcsThreads()
          || a.GetComm() != b.GetComm()
        ;
      }
    ) == std::end(addresses) );
    emp_assert( !addresses.empty() );

    auto backend = std::make_shared<
      typename AggregatorSpec::ProcBackEnd
    >();

    auto source = uit::Source<AggregatorSpec>{
      std::in_place_type_t<
        typename AggregatorSpec::ProcOutletDuct
      >{},
      addresses.front(),
      backend
    };

    backend->Initialize();

    outlet = source.GetOutlet();

    std::transform(
      std::begin(addresses),
      std::end(addresses),
      std::inserter( buffer, std::end(buffer) ),
      [](const auto& address){
        return std::pair{ address.GetTag(), value_type{} };
      }
    );

    std::transform(
      std::begin(addresses),
      std::end(addresses),
      std::inserter( hit_since_last_consume, std::end(hit_since_last_consume) ),
      [](const auto& address){
        return std::pair{ address.GetTag(), true };
      }
    );

    emp_assert( IsInitialized() );

  }

};

} // namespace uit

#endif // #ifndef UIT_DUCTS_PROC_IMPL_BACKEND_IMPL_OUTLETMEMORYAGGREGATOR_HPP_INCLUDE
