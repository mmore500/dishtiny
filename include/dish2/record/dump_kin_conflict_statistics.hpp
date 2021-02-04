#pragma once
#ifndef DISH2_RECORD_DUMP_KIN_CONFLICT_STATISTICS_HPP_INCLUDE
#define DISH2_RECORD_DUMP_KIN_CONFLICT_STATISTICS_HPP_INCLUDE

#include <algorithm>

#include "../../../third-party/conduit/include/uitsl/algorithm/for_each.hpp"
#include "../../../third-party/Empirical/include/emp/data/DataFile.hpp"
#include "../../../third-party/header-only-gzstream/include/hogzstr/gzstream.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/utility/CountingIterator.hpp"

#include "../config/has_replicate.hpp"
#include "../config/has_series.hpp"
#include "../config/has_stint.hpp"
#include "../introspection/count_spawn_events.hpp"
#include "../introspection/get_total_spawn_event_kin_neighbors.hpp"
#include "../introspection/get_total_spawn_event_neighbors.hpp"

#include "make_filename/make_kin_conflict_statistics_filename.hpp"

namespace dish2 {

template< typename Spec >
void dump_kin_conflict_statistics(
  const dish2::ThreadWorld< Spec >& world, const size_t thread_idx
) {

  emp::DataFile file(
    dish2::make_kin_conflict_statistics_filename( thread_idx )
  );

  if ( dish2::has_stint() ) file.AddVal(cfg.STINT(), "Stint");
  if ( dish2::has_series() ) file.AddVal(cfg.SERIES(), "Series");
  if ( dish2::has_replicate() ) file.AddVal(cfg.REPLICATE(), "Replicate");

  size_t kin_id_commonality_parent_eliminated;
  size_t num_parents;
  size_t total_parent_neighbors;
  size_t total_parent_kin_neighbors;
  size_t total_kin_eliminated;

  double fraction_eliminated_that_are_kin;
  double fraction_neighbors_that_are_kin;
  double kin_conflict_ratio;

  file.AddVar(
    kin_id_commonality_parent_eliminated, "Parent-Eliminated Kin ID Commonality"
  );

  file.AddVar( num_parents, "Num Parents" );
  file.AddVar( total_parent_neighbors, "Total Num Parent Neighbors" );
  file.AddVar( total_parent_kin_neighbors, "Total Num Parent Kin Neighbors" );
  file.AddVar( total_kin_eliminated, "Total Num Kin Eliminated" );

  file.AddVar(
    fraction_eliminated_that_are_kin, "Fraction Eliminated that are Kin"
  );
  file.AddVar(
    fraction_neighbors_that_are_kin, "Fraction Neighbors that are Kin"
  );
  file.AddVar( kin_conflict_ratio, "Kin Conflict Ratio" );

  file.PrintHeaderKeys();

  for (
    kin_id_commonality_parent_eliminated = 1;
    kin_id_commonality_parent_eliminated < Spec::NLEV + 1;
    ++kin_id_commonality_parent_eliminated
  ) {
    num_parents = dish2::count_spawn_events<Spec>(
      world, kin_id_commonality_parent_eliminated
    );
    total_parent_neighbors
      = dish2::get_total_spawn_event_neighbors<Spec>(
        world, kin_id_commonality_parent_eliminated
      );
    const size_t highest_shared_kin_id_level
      = kin_id_commonality_parent_eliminated - 1;
    total_parent_kin_neighbors
      = dish2::get_total_spawn_event_kin_neighbors<Spec>(
        world, highest_shared_kin_id_level, kin_id_commonality_parent_eliminated
      );
    total_kin_eliminated
      = dish2::get_total_spawn_event_kin_eliminated<Spec>(
        world, kin_id_commonality_parent_eliminated
      );

    fraction_neighbors_that_are_kin =
      total_parent_kin_neighbors
      / static_cast<double>( total_parent_neighbors );
    fraction_eliminated_that_are_kin =
      total_kin_eliminated / static_cast<double>( num_parents );
    kin_conflict_ratio =
      fraction_eliminated_that_are_kin / fraction_neighbors_that_are_kin;


    file.Update();
  }

}

} // namespace dish2

#endif // #ifndef DISH2_RECORD_DUMP_KIN_CONFLICT_STATISTICS_HPP_INCLUDE
