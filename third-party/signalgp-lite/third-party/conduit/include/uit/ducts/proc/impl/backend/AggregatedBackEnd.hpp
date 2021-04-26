#pragma once
#ifndef UIT_DUCTS_PROC_IMPL_BACKEND_AGGREGATEDBACKEND_HPP_INCLUDE
#define UIT_DUCTS_PROC_IMPL_BACKEND_AGGREGATEDBACKEND_HPP_INCLUDE

#include <tuple>

#include "../../../../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../../../../third-party/Empirical/include/emp/datastructs/tuple_utils.hpp"
#include "../../../../../../third-party/Empirical/third-party/robin-hood-hashing/src/include/robin_hood.h"

#include "../../../../setup/InterProcAddress.hpp"

#include "impl/AggregatorSpec.hpp"
#include "impl/InletMemoryAggregator.hpp"
#include "impl/OutletMemoryAggregator.hpp"

namespace uit {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
// TODO use templating to share an implementation with PooledBackEnd
template<
  typename ImplSpec,
  template<typename> typename ProcDuct
>
class AggregatedBackEnd {

  using T = typename ImplSpec::T;

  using address_t = uit::InterProcAddress;

  using AggregatorSpec_t = uit::AggregatorSpec<ImplSpec, ProcDuct>;

public:

  using inlet_aggregator_t = uit::InletMemoryAggregator<AggregatorSpec_t>;
  using outlet_aggregator_t = uit::OutletMemoryAggregator<AggregatorSpec_t>;

private:

  // < caller/target thread/proc > -> inlet aggregator
  robin_hood::unordered_map<
    std::tuple<
      uitsl::proc_id_t, uitsl::thread_id_t,
      uitsl::proc_id_t, uitsl::thread_id_t
    >,
    inlet_aggregator_t,
    emp::TupleHash<
      uitsl::proc_id_t, uitsl::thread_id_t,
      uitsl::proc_id_t, uitsl::thread_id_t
    >
  > inlet_aggregators;

  // < caller/target thread/proc > -> outlet aggregator
  robin_hood::unordered_map<
    std::tuple<
      uitsl::proc_id_t, uitsl::thread_id_t,
      uitsl::proc_id_t, uitsl::thread_id_t
    >,
    outlet_aggregator_t,
    emp::TupleHash<
      uitsl::proc_id_t, uitsl::thread_id_t,
      uitsl::proc_id_t, uitsl::thread_id_t
    >
  > outlet_aggregators;

  bool AreAllInletAggregatorsInitialized() const {

    // check that all windows are in the same initialization state
    emp_assert( std::adjacent_find(
      std::begin(inlet_aggregators), std::end(inlet_aggregators),
      [](const auto& aggregator_pair1, const auto& aggregator_pair2) {
        const auto& [key1, aggregator1] = aggregator_pair1;
        const auto& [key2, aggregator2] = aggregator_pair2;
        return aggregator1.IsInitialized() != aggregator2.IsInitialized();
      }
    ) == std::end(inlet_aggregators) );


    return std::any_of(
      std::begin(inlet_aggregators), std::end(inlet_aggregators),
      [](const auto& aggregator_pair) {
        const auto& [key, aggregator] = aggregator_pair;
        return aggregator.IsInitialized();
      }
    );

  }

  bool AreAllOutletAggregatorsInitialized() const {

    // check that all windows are in the same initialization state
    emp_assert( std::adjacent_find(
      std::begin(outlet_aggregators), std::end(outlet_aggregators),
      [](const auto& aggregator_pair1, const auto& aggregator_pair2) {
        const auto& [key1, aggregator1] = aggregator_pair1;
        const auto& [key2, aggregator2] = aggregator_pair2;
        return aggregator1.IsInitialized() != aggregator2.IsInitialized();
      }
    ) == std::end(outlet_aggregators) );


    return std::any_of(
      std::begin(outlet_aggregators), std::end(outlet_aggregators),
      [](const auto& aggregator_pair) {
        const auto& [key, aggregator] = aggregator_pair;
        return aggregator.IsInitialized();
      }
    );

  }

  bool IsInitialized() const {
    emp_assert(
      AreAllInletAggregatorsInitialized() == AreAllOutletAggregatorsInitialized()
      || inlet_aggregators.empty()
      || outlet_aggregators.empty()
    );
    return AreAllInletAggregatorsInitialized()
    || AreAllOutletAggregatorsInitialized();
  }

  bool IsEmpty() const {
    return outlet_aggregators.empty() && inlet_aggregators.empty();
  }

public:

  void RegisterInletSlot(const address_t& address) {
    emp_assert( !IsInitialized() );
    inlet_aggregators[ address.WhichProcsThreads() ].Register(address);
  }

  void RegisterOutletSlot(const address_t& address) {
    emp_assert( !IsInitialized() );
    outlet_aggregators[ address.WhichProcsThreads() ].Register(address);
  }

  void Initialize() {
    emp_assert( !IsInitialized() );

    for (auto& [__, aggregator] : inlet_aggregators) aggregator.Initialize();
    for (auto& [__, aggregator] : outlet_aggregators) aggregator.Initialize();

    emp_assert( IsInitialized() || IsEmpty() );
  }

  inlet_aggregator_t& GetInletAggregator(const address_t& address) {
    emp_assert( IsInitialized() );

    auto& aggregator = inlet_aggregators.at( address.WhichProcsThreads() );

    emp_assert( aggregator.IsInitialized(), aggregator.GetSize() );

    return aggregator;
  }

  outlet_aggregator_t& GetOutletAggregator(const address_t& address) {
    emp_assert( IsInitialized() );

    auto& aggregator = outlet_aggregators.at( address.WhichProcsThreads() );

    emp_assert( aggregator.IsInitialized() );

    return aggregator;
  }

  constexpr static bool CanStep() { return outlet_aggregator_t::CanStep(); }

};

} // namespace uit

#endif // #ifndef UIT_DUCTS_PROC_IMPL_BACKEND_AGGREGATEDBACKEND_HPP_INCLUDE
