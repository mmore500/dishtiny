#pragma once
#ifndef DISH2_SERVICES_INTERPRETEDINTROSPECTIVESTATEREFRESHSERVICE_HPP_INCLUDE
#define DISH2_SERVICES_INTERPRETEDINTROSPECTIVESTATEREFRESHSERVICE_HPP_INCLUDE

#include <algorithm>

#include "../../../third-party/conduit/include/uitsl/algorithm/transform.hpp"
#include "../../../third-party/conduit/include/uitsl/debug/WarnOnce.hpp"
#include "../../../third-party/conduit/include/uitsl/math/shift_mod.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/utility/CountingIterator.hpp"

#include "../cell/cardinal_iterators/CellAgeWrapper.hpp"
#include "../cell/cardinal_iterators/EpochWrapper.hpp"
#include "../cell/cardinal_iterators/IsChildCellOfWrapper.hpp"
#include "../cell/cardinal_iterators/IsChildGroupOfWrapper.hpp"
#include "../cell/cardinal_iterators/IsNewbornWrapper.hpp"
#include "../cell/cardinal_iterators/IsParentCellOfWrapper.hpp"
#include "../cell/cardinal_iterators/IsParentGroupOfWrapper.hpp"
#include "../cell/cardinal_iterators/KinGroupAgeWrapper.hpp"
#include "../cell/cardinal_iterators/KinGroupIDAncestorViewWrapper.hpp"
#include "../cell/cardinal_iterators/KinGroupIDViewWrapper.hpp"
#include "../cell/cardinal_iterators/KinGroupWillExpireWrapper.hpp"
#include "../cell/cardinal_iterators/MostRecentCauseOfDeathWrapper.hpp"
#include "../cell/cardinal_iterators/NeighborApoptosisWrapper.hpp"
#include "../cell/cardinal_iterators/NeighborCellAgeWrapper.hpp"
#include "../cell/cardinal_iterators/NeighborCellAgeWrapper.hpp"
#include "../cell/cardinal_iterators/NeighborFragmentedWrapper.hpp"
#include "../cell/cardinal_iterators/NeighborIsNewbornWrapper.hpp"
#include "../cell/cardinal_iterators/NeighborKinGroupAgeWrapper.hpp"
#include "../cell/cardinal_iterators/NeighborKinGroupIDAncestorViewWrapper.hpp"
#include "../cell/cardinal_iterators/NeighborKinGroupIDViewWrapper.hpp"
#include "../cell/cardinal_iterators/NeighborKinGroupWillExpireWrapper.hpp"
#include "../cell/cardinal_iterators/NeighborMostRecentCauseOfDeathWrapper.hpp"
#include "../cell/cardinal_iterators/NeighborNumKnownQuorumBitsWrapper.hpp"
#include "../cell/cardinal_iterators/NeighborOptimumQuorumExceededWrapper.hpp"
#include "../cell/cardinal_iterators/NeighborPhylogeneticRootViewWrapper.hpp"
#include "../cell/cardinal_iterators/NeighborResourceStockpileWrapper.hpp"
#include "../cell/cardinal_iterators/NeighborSpawnedFromWrapper.hpp"
#include "../cell/cardinal_iterators/NumKnownQuorumBitsWrapper.hpp"
#include "../cell/cardinal_iterators/OptimumQuorumExceededWrapper.hpp"
#include "../cell/cardinal_iterators/ParentFragmentedWrapper.hpp"
#include "../cell/cardinal_iterators/PhylogeneticRootMatchWrapper.hpp"
#include "../cell/cardinal_iterators/PhylogeneticRootViewWrapper.hpp"
#include "../cell/cardinal_iterators/ResourceStockpileWrapper.hpp"
#include "../cell/cardinal_iterators/RicherThanNeighborWrapper.hpp"
#include "../cell/cardinal_iterators/SpawnedFromWrapper.hpp"
#include "../cell/cardinal_iterators/StockpileDepletedWrapper.hpp"
#include "../cell/cardinal_iterators/StockpileFecundWrapper.hpp"
#include "../config/cfg.hpp"
#include "../debug/LogScope.hpp"

namespace dish2 {

/**
 * Refreshes the interpreted introspective state of a cell.
 */
class InterpretedIntrospectiveStateRefreshService {

  template< typename Cell >
  static void RefreshIsChildCellOf( Cell& cell ) {
    using spec_t = typename Cell::spec_t;
    uitsl::transform(
      cell.template begin<dish2::CellAgeWrapper<spec_t>>(),
      cell.template end<dish2::CellAgeWrapper<spec_t>>(),
      cell.template begin<dish2::NeighborCellAgeWrapper<spec_t>>(),
      cell.template begin<dish2::SpawnedFromWrapper<spec_t>>(),
      cell.template begin<dish2::IsParentCellOfWrapper<spec_t>>(),
      [](
        const size_t my_age, const size_t neighbor_age, const bool spawned_from
      ){
        return spawned_from && my_age < neighbor_age;
      }
    );
  }

  template< typename Cell >
  static void RefreshIsChildGroupOf( Cell& cell ) {
    using spec_t = typename Cell::spec_t;
    uitsl::for_each(
      cell.template begin<dish2::KinGroupIDAncestorViewWrapper<spec_t>>(),
      cell.template end<dish2::KinGroupIDAncestorViewWrapper<spec_t>>(),
      cell.template begin<dish2::NeighborKinGroupIDViewWrapper<spec_t>>(),
      cell.template begin<dish2::IsChildGroupOfWrapper<spec_t>>(),
      [](
        const auto& my_ancestor_ids, const auto& neighbor_ids,
        auto& is_child_group_of
      ){
        std::transform(
          std::begin(my_ancestor_ids), std::end(my_ancestor_ids),
          std::begin(neighbor_ids),
          std::begin(is_child_group_of),
          []( const auto& my_ancestor, const auto& neighbor_id ){
            return my_ancestor == neighbor_id;
          }
        );
      }
    );
  }

  template< typename Cell >
  static void RefreshIsNewborn( Cell& cell ) {
    using spec_t = typename Cell::spec_t;
    std::transform(
      cell.template begin<dish2::CellAgeWrapper<spec_t>>(),
      cell.template end<dish2::CellAgeWrapper<spec_t>>(),
      cell.template begin<dish2::IsNewbornWrapper<spec_t>>(),
      []( const size_t my_age ){
        return my_age <= dish2::cfg.EVENT_LAUNCHING_SERVICE_FREQUENCY();
      }
    );
  }

  template< typename Cell >
  static void RefreshIsParentCellOf( Cell& cell ) {
    using spec_t = typename Cell::spec_t;
    uitsl::transform(
      cell.template begin<dish2::CellAgeWrapper<spec_t>>(),
      cell.template end<dish2::CellAgeWrapper<spec_t>>(),
      cell.template begin<dish2::NeighborCellAgeWrapper<spec_t>>(),
      cell.template begin<dish2::NeighborSpawnedFromWrapper<spec_t>>(),
      cell.template begin<dish2::IsChildCellOfWrapper<spec_t>>(),
      [](
        const size_t my_age, const size_t neighbor_age,
        const bool neighbor_spawned_from
      ){
        return neighbor_spawned_from && my_age > neighbor_age;
      }
    );

  }

  template< typename Cell >
  static void RefreshIsParentGroupOf( Cell& cell ) {
    using spec_t = typename Cell::spec_t;
    uitsl::for_each(
      cell.template begin<dish2::KinGroupIDViewWrapper<spec_t>>(),
      cell.template end<dish2::KinGroupIDViewWrapper<spec_t>>(),
      cell.template begin<dish2::NeighborKinGroupIDAncestorViewWrapper<spec_t>>(),
      cell.template begin<dish2::IsParentGroupOfWrapper<spec_t>>(),
      [](
        const auto& my_ids, const auto& neighbor_ancestor_ids,
        auto& is_parent_group_of
      ){
        std::transform(
          std::begin(my_ids), std::end(my_ids),
          std::begin(neighbor_ancestor_ids),
          std::begin(is_parent_group_of),
          []( const auto& my_id, const auto& neighbor_ancestor ){
            return my_id == neighbor_ancestor;
          }
        );
      }
    );
  }


  template< typename Cell >
  static void RefreshNeighborIsNewborn( Cell& cell ) {
    using spec_t = typename Cell::spec_t;
    std::transform(
      cell.template begin<dish2::NeighborCellAgeWrapper<spec_t>>(),
      cell.template end<dish2::NeighborCellAgeWrapper<spec_t>>(),
      cell.template begin<dish2::NeighborIsNewbornWrapper<spec_t>>(),
      []( const size_t neighbor_age ){
        return neighbor_age <= dish2::cfg.EVENT_LAUNCHING_SERVICE_FREQUENCY();
      }
    );
  }


  template< typename Cell >
  static void RefreshKinGroupWillExpire( Cell& cell ) {
    using spec_t = typename Cell::spec_t;
    uitsl::for_each(
      cell.template begin<dish2::KinGroupAgeWrapper<spec_t>>(),
      cell.template end<dish2::KinGroupAgeWrapper<spec_t>>(),
      cell.template begin<dish2::KinGroupWillExpireWrapper<spec_t>>(),
      [](
        const auto& kin_group_ages, auto& kin_group_will_expires
      ){
        std::transform(
          std::begin(kin_group_ages), std::end(kin_group_ages),
          std::begin( cfg.GROUP_EXPIRATION_DURATIONS() ),
          std::begin(kin_group_will_expires),
          []( const size_t age, const size_t duration ){
            return age > 0.8 * duration;
          }
        );
      }
    );
  }

  template< typename Cell >
  static void RefreshNeighborKinGroupWillExpire( Cell& cell ) {
    using spec_t = typename Cell::spec_t;
    uitsl::for_each(
      cell.template begin<dish2::NeighborKinGroupAgeWrapper<spec_t>>(),
      cell.template end<dish2::NeighborKinGroupAgeWrapper<spec_t>>(),
      cell.template begin<dish2::NeighborKinGroupWillExpireWrapper<spec_t>>(),
      [](
        const auto& kin_group_ages, auto& kin_group_will_expires
      ){
        std::transform(
          std::begin(kin_group_ages), std::end(kin_group_ages),
          std::begin( cfg.GROUP_EXPIRATION_DURATIONS() ),
          std::begin(kin_group_will_expires),
          []( const size_t age, const size_t duration ){
            return age > 0.8 * duration;
          }
        );
      }
    );
  }

  template< typename Cell >
  static void RefreshNeighborOptimumQuorumExceeded( Cell& cell ) {
    using spec_t = typename Cell::spec_t;
    uitsl::for_each(
      cell.template begin<dish2::NeighborNumKnownQuorumBitsWrapper<spec_t>>(),
      cell.template end<dish2::NeighborNumKnownQuorumBitsWrapper<spec_t>>(),
      cell.template begin<dish2::NeighborOptimumQuorumExceededWrapper<spec_t>>(),
      [](
        const auto& num_known_quorum_bits, auto& optimum_quorum_exceeded
      ){
        std::transform(
          std::begin(num_known_quorum_bits), std::end(num_known_quorum_bits),
          std::begin( cfg.OPTIMAL_QUORUM_COUNT() ),
          std::begin( optimum_quorum_exceeded ),
          []( const size_t known_bits, const size_t optimum ){
            return known_bits > optimum;
          }
        );
      }
    );
  }

  template< typename Cell >
  static void RefreshOptimumQuorumExceeded( Cell& cell ) {
    using spec_t = typename Cell::spec_t;
    uitsl::for_each(
      cell.template begin<dish2::NumKnownQuorumBitsWrapper<spec_t>>(),
      cell.template end<dish2::NumKnownQuorumBitsWrapper<spec_t>>(),
      cell.template begin<dish2::OptimumQuorumExceededWrapper<spec_t>>(),
      [](
        const auto& num_known_quorum_bits, auto& optimum_quorum_exceeded
      ){
        std::transform(
          std::begin(num_known_quorum_bits), std::end(num_known_quorum_bits),
          std::begin( cfg.OPTIMAL_QUORUM_COUNT() ),
          std::begin( optimum_quorum_exceeded ),
          []( const size_t known_bits, const size_t optimum ){
            return known_bits > optimum;
          }
        );
      }
    );
  }

  template< typename Cell >
  static void RefreshParentFragmented( Cell& cell ) {
    using spec_t = typename Cell::spec_t;
    std::transform(
      cell.template begin<dish2::MostRecentCauseOfDeathWrapper<spec_t>>(),
      cell.template end<dish2::MostRecentCauseOfDeathWrapper<spec_t>>(),
      cell.template begin<dish2::ParentFragmentedWrapper<spec_t>>(),
      []( const char cause_of_death ){
        return cause_of_death
          == static_cast<char>(dish2::CauseOfDeath::fragmentation);
      }
    );
  }

  template< typename Cell >
  static void RefreshNeighborFragmented( Cell& cell ) {
    using spec_t = typename Cell::spec_t;
    std::transform(
      cell.template begin<dish2::NeighborMostRecentCauseOfDeathWrapper<spec_t>>(),
      cell.template end<dish2::NeighborMostRecentCauseOfDeathWrapper<spec_t>>(),
      cell.template begin<dish2::NeighborFragmentedWrapper<spec_t>>(),
      []( const char cause_of_death ){
        return cause_of_death
          == static_cast<char>(dish2::CauseOfDeath::fragmentation);
      }
    );
  }

  template< typename Cell >
  static void RefreshNeighborApoptosis( Cell& cell ) {
    using spec_t = typename Cell::spec_t;
    std::transform(
      cell.template begin<dish2::NeighborMostRecentCauseOfDeathWrapper<spec_t>>(),
      cell.template end<dish2::NeighborMostRecentCauseOfDeathWrapper<spec_t>>(),
      cell.template begin<dish2::NeighborApoptosisWrapper<spec_t>>(),
      []( const char cause_of_death ){
        return cause_of_death
          == static_cast<char>(dish2::CauseOfDeath::apoptosis);
      }
    );
  }

  template< typename Cell >
  static void RefreshPhylogeneticRootMatch( Cell& cell ) {
    using spec_t = typename Cell::spec_t;
    std::transform(
      cell.template begin<dish2::PhylogeneticRootViewWrapper<spec_t>>(),
      cell.template end<dish2::PhylogeneticRootViewWrapper<spec_t>>(),
      cell.template begin<dish2::NeighborPhylogeneticRootViewWrapper<spec_t>>(),
      cell.template begin<dish2::PhylogeneticRootMatchWrapper<spec_t>>(),
      []( const size_t my_root, const size_t neighbor_root ){
        return my_root == neighbor_root;
      }
    );
  }


  template< typename Cell >
  static void RefreshRicherThanNeighbor( Cell& cell ) {
    using spec_t = typename Cell::spec_t;
    std::transform(
      cell.template begin<dish2::ResourceStockpileWrapper<spec_t>>(),
      cell.template end<dish2::ResourceStockpileWrapper<spec_t>>(),
      cell.template begin<dish2::NeighborResourceStockpileWrapper<spec_t>>(),
      cell.template begin<dish2::RicherThanNeighborWrapper<spec_t>>(),
      []( const float my_resource, const float neighbor_resource ){
        return my_resource > neighbor_resource;
      }
    );
  }

  template< typename Cell >
  static void RefreshStockpileDepleted( Cell& cell ) {
    using spec_t = typename Cell::spec_t;
    std::transform(
      cell.template begin<dish2::ResourceStockpileWrapper<spec_t>>(),
      cell.template end<dish2::ResourceStockpileWrapper<spec_t>>(),
      cell.template begin<dish2::StockpileDepletedWrapper<spec_t>>(),
      []( const float stockpile ){
        return stockpile < 2 * dish2::cfg.BASE_HARVEST_RATE();
      }
    );
  }

  template< typename Cell >
  static void RefreshStockpileFecund( Cell& cell ) {
    using spec_t = typename Cell::spec_t;
    std::transform(
      cell.template begin<dish2::ResourceStockpileWrapper<spec_t>>(),
      cell.template end<dish2::ResourceStockpileWrapper<spec_t>>(),
      cell.template begin<dish2::StockpileFecundWrapper<spec_t>>(),
      []( const float stockpile ){
        return stockpile >= 1.0;
      }
    );
  }


public:

  static bool ShouldRun( const size_t update, const bool alive ) {
    const size_t freq = dish2::cfg.EVENT_LAUNCHING_SERVICE_FREQUENCY();
    return
      alive
      && freq > 0
      && uitsl::shift_mod( update, freq ) == 0;
  }

  template<typename Cell>
  static void DoService( Cell& cell ) {

    const dish2::LogScope guard{
      "interpreted introspective state refresh service", "TODO", 3
    };

    RefreshIsChildCellOf<Cell>( cell );
    RefreshIsChildGroupOf<Cell>( cell );
    RefreshIsNewborn<Cell>( cell );
    RefreshIsParentCellOf<Cell>( cell );
    RefreshIsParentGroupOf<Cell>( cell );
    RefreshNeighborIsNewborn<Cell>( cell );
    RefreshKinGroupWillExpire<Cell>( cell );
    RefreshNeighborKinGroupWillExpire<Cell>( cell );
    RefreshNeighborOptimumQuorumExceeded<Cell>( cell );
    RefreshOptimumQuorumExceeded<Cell>( cell );
    RefreshParentFragmented<Cell>( cell );
    RefreshPhylogeneticRootMatch<Cell>( cell );
    RefreshNeighborFragmented<Cell>( cell );
    RefreshNeighborApoptosis<Cell>( cell );
    RefreshRicherThanNeighbor<Cell>( cell );

  }

};

} // namespace dish2

#endif // #ifndef DISH2_SERVICES_INTERPRETEDINTROSPECTIVESTATEREFRESHSERVICE_HPP_INCLUDE
