#pragma once
#ifndef DISH2_RECORD_DUMP_KIN_CONFLICT_STATISTICS_HPP_INCLUDE
#define DISH2_RECORD_DUMP_KIN_CONFLICT_STATISTICS_HPP_INCLUDE

#include "../../../third-party/conduit/include/uitsl/algorithm/for_each.hpp"
#include "../../../third-party/conduit/include/uitsl/mpi/comm_utils.hpp"
#include "../../../third-party/Empirical/include/emp/data/DataFile.hpp"
#include "../../../third-party/Empirical/include/emp/tools/keyname_utils.hpp"
#include "../../../third-party/Empirical/include/emp/tools/string_utils.hpp"
#include "../../../third-party/header-only-gzstream/include/hogzstr/gzstream.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/utility/CountingIterator.hpp"

#include "../config/has_replicate.hpp"
#include "../config/has_series.hpp"
#include "../config/has_stint.hpp"
#include "../debug/log_msg.hpp"
#include "../introspection/count_kin_neighbors.hpp"
#include "../introspection/count_live_cardinals.hpp"
#include "../introspection/count_spawn_events.hpp"
#include "../introspection/get_total_spawn_event_kin_eliminated.hpp"
#include "../introspection/get_total_spawn_event_kin_neighbors.hpp"
#include "../introspection/get_total_spawn_event_neighbors.hpp"
#include "../utility/pare_keyname_filename.hpp"

#include "make_filename/make_data_path.hpp"
#include "make_filename/make_kin_conflict_statistics_filename.hpp"

namespace dish2 {

template< typename Spec >
void dump_kin_conflict_statistics( const dish2::ThreadWorld< Spec >& world ) {

  const std::string out_filename = dish2::pare_keyname_filename(
    dish2::make_kin_conflict_statistics_filename(),
    dish2::make_data_path()
  );

  emp::DataFile file( dish2::make_data_path( out_filename ) );

  if ( dish2::has_stint() ) file.AddVal(cfg.STINT(), "Stint");
  if ( dish2::has_series() ) file.AddVal(cfg.SERIES(), "Series");
  if ( dish2::has_replicate() ) file.AddVal(cfg.REPLICATE(), "Replicate");
  file.AddVal(cfg.TREATMENT(), "Treatment");
  if ( cfg.TREATMENT().find('=') != std::string::npos ) {
    for ( const auto& [k, v] : emp::keyname::unpack( cfg.TREATMENT() ) ) {
      file.AddVal( v, emp::to_string("Treatment ", k) );
    }
  }
  file.AddVal( uitsl::get_proc_id(), "proc" );
  file.AddVal( dish2::thread_idx, "thread" );

  size_t kin_id_commonality_parent_eliminated;

  const size_t num_spawn_events = dish2::count_spawn_events<Spec>( world );
  const size_t total_neighbors = dish2::count_live_cardinals<Spec>( world );
  const size_t total_spawn_event_neighbors
    = dish2::get_total_spawn_event_neighbors<Spec>( world );

  size_t total_neighbors_that_are_kin;
  size_t total_spawn_event_neighbors_that_are_kin;
  size_t total_kin_eliminated;

  double fraction_eliminated_that_are_kin;
  double fraction_spawn_event_neighbors_that_are_kin;
  double fraction_neighbors_that_are_kin;
  double spawn_event_kin_conflict_ratio;
  double kin_conflict_ratio;

  file.AddVar(
    kin_id_commonality_parent_eliminated, "Parent-Eliminated Kin ID Commonality"
  );

  file.AddVar( num_spawn_events, "Num Spawn Events" );
  file.AddVar( total_neighbors, "Total Num Neighbors" );
  file.AddVar( total_neighbors_that_are_kin, "Total Num Neighbors that are Kin" );
  file.AddVar( total_spawn_event_neighbors, "Total Num Spawn Event Neighbors" );
  file.AddVar( total_spawn_event_neighbors_that_are_kin, "Total Num Spawn Event Neighbors that are Kin" );
  file.AddVar( total_kin_eliminated, "Total Num Kin Eliminated" );

  file.AddVar(
    fraction_eliminated_that_are_kin, "Fraction Eliminated that are Kin"
  );
  file.AddVar(
    fraction_spawn_event_neighbors_that_are_kin,
    "Fraction Spawn Event Neighbors that are Kin"
  );
  file.AddVar(
    fraction_neighbors_that_are_kin,
    "Fraction Neighbors that are Kin"
  );
  file.AddVar( kin_conflict_ratio, "Kin Conflict Ratio" );
  file.AddVar( spawn_event_kin_conflict_ratio, "Kin Conflict at Spawn Ratio" );

  file.PrintHeaderKeys();

  for ( size_t kin_id_level{}; kin_id_level < Spec::NLEV; ++kin_id_level ) {

    kin_id_commonality_parent_eliminated = kin_id_level + 1;

    total_neighbors_that_are_kin
      = dish2::count_kin_neighbors( world, kin_id_level );
    total_spawn_event_neighbors_that_are_kin
      = dish2::get_total_spawn_event_kin_neighbors<Spec>( world, kin_id_level );

    total_kin_eliminated = dish2::get_total_spawn_event_kin_eliminated<Spec>(
        world, kin_id_commonality_parent_eliminated
      );

    fraction_neighbors_that_are_kin = total_neighbors_that_are_kin
      / static_cast<double>( total_neighbors );
    fraction_spawn_event_neighbors_that_are_kin =
      total_spawn_event_neighbors_that_are_kin
      / static_cast<double>( total_spawn_event_neighbors );

    fraction_eliminated_that_are_kin =
      total_kin_eliminated / static_cast<double>( num_spawn_events );

    spawn_event_kin_conflict_ratio =
      fraction_eliminated_that_are_kin / fraction_spawn_event_neighbors_that_are_kin;
    kin_conflict_ratio =
      fraction_eliminated_that_are_kin / fraction_neighbors_that_are_kin;

    file.Update();
  }

  dish2::log_msg( "dumped kin conflict statistics" );

}

} // namespace dish2

#endif // #ifndef DISH2_RECORD_DUMP_KIN_CONFLICT_STATISTICS_HPP_INCLUDE
