#pragma once
#ifndef DISH2_RECORD_DUMP_INTERROOT_PHENOTYPE_DIFFERENTIATION_HPP_INCLUDE
#define DISH2_RECORD_DUMP_INTERROOT_PHENOTYPE_DIFFERENTIATION_HPP_INCLUDE

#include <algorithm>
#include <fstream>
#include <string>
#include <utility>

#include "../../../third-party/conduit/include/uitsl/meta/function_cast.hpp"
#include "../../../third-party/conduit/include/uitsl/utility/keyname_key_union.hpp"
#include "../../../third-party/Empirical/include/emp/data/DataFile.hpp"
#include "../../../third-party/Empirical/include/emp/tools/keyname_utils.hpp"
#include "../../../third-party/Empirical/include/emp/tools/string_utils.hpp"
#include "../../../third-party/lazy-iterators/include/lazyit/distinct_pairs.hpp"

#include "../algorithm/detect_phenotypic_divergence.hpp"
#include "../config/get_endeavor.hpp"
#include "../config/get_repro.hpp"
#include "../config/get_slurm_job_id.hpp"
#include "../config/has_replicate.hpp"
#include "../config/has_series.hpp"
#include "../config/has_stint.hpp"
#include "../introspection/any_live_cells.hpp"
#include "../introspection/count_live_cells.hpp"
#include "../introspection/get_root_id_abundance.hpp"
#include "../introspection/get_root_id_count.hpp"
#include "../introspection/get_root_id_prevalence.hpp"
#include "../load/count_root_ids.hpp"
#include "../load/get_innoculum_filename.hpp"
#include "../load/get_innoculum_filenames.hpp"
#include "../load/get_innoculum_slug.hpp"
#include "../load/get_root_ids.hpp"
#include "../utility/calc_fitness_differential.hpp"
#include "../utility/pare_keyname_filename.hpp"
#include "../world/ThreadWorld.hpp"

#include "make_filename/make_data_path.hpp"
#include "make_filename/make_interroot_phenotype_differentiation_filename.hpp"

namespace dish2 {

template< typename Spec >
void dump_interroot_phenotype_differentiation(
  const dish2::ThreadWorld< Spec >& world
) {

  const std::string out_filename = dish2::pare_keyname_filename(
    dish2::make_interroot_phenotype_differentiation_filename(),
    dish2::make_data_path()
  );

  emp::DataFile file( dish2::make_data_path( out_filename ) );

  if ( dish2::has_stint() ) file.AddVal(cfg.STINT(), "Test Stint");
  if ( dish2::has_series() ) file.AddVal(cfg.SERIES(), "Test Series");
  if ( dish2::has_replicate() ) file.AddVal(
    cfg.REPLICATE(), "Test Replicate"
  );
  file.AddVal( cfg.TREATMENT(), "Test Treatment" );
  if ( cfg.TREATMENT().find('=') != std::string::npos ) {
    for ( const auto& [k, v] : emp::keyname::unpack( cfg.TREATMENT() ) ) {
      file.AddVal( v, emp::to_string("Treatment ", k) );
    }
  }

  if ( dish2::get_endeavor() ) file.AddVal(
    *dish2::get_endeavor(), "Test Endeavor"
  );
  if ( dish2::get_repro() ) file.AddVal(
    *dish2::get_repro(), "Test Repro"
  );
  if ( dish2::get_slurm_job_id() ) file.AddVal(
    *dish2::get_slurm_job_id(), "Competition Slurm Job ID"
  );
  file.AddVal( dish2::thread_idx, "Test Thread" );
  file.AddVal( uitsl::get_proc_id(), "Test Process" );
  file.AddVal( world.GetUpdate(), "Update" );

  std::pair<std::string, std::string> filename;
  std::pair<std::string, std::string> slug;
  std::pair<size_t, size_t> root_id;
  bool phenotype_divergence_detected;

  file.AddVar(root_id.first, "First Root ID");
  file.AddVar(root_id.second, "Second Root ID");
  file.AddVar(slug.first, "First Genome Slug");
  file.AddVar(slug.second, "Second Genome Slug");
  file.AddVar(filename.first, "First Genome Filename");
  file.AddVar(filename.second, "Second Genome Filename");

  file.AddVar(phenotype_divergence_detected, "Phenotype Divergence Detected");

  for (
    const auto& key
    : uitsl::keyname_key_union( dish2::get_innoculum_filenames() )
  ) if (key != "_" ) file.AddFun( uitsl::function_cast( [key, &filename](){
    // unpacks /current/ filename for specified key
    const auto attrs = emp::keyname::unpack( filename.first );
    const auto res = attrs.find( key );
    return res != std::end( attrs ) ? res->second : "";
  } ), emp::to_string("first genome ", key) );

  for (
    const auto& key
    : uitsl::keyname_key_union( dish2::get_innoculum_filenames() )
  ) if (key != "_" ) file.AddFun( uitsl::function_cast( [key, &filename](){
    // unpacks /current/ filename for specified key
    const auto attrs = emp::keyname::unpack( filename.second );
    const auto res = attrs.find( key );
    return res != std::end( attrs ) ? res->second : "";
  } ), emp::to_string("second genome ", key) );

  file.PrintHeaderKeys();

  auto root_ids = dish2::get_root_ids();
  std::sort( std::begin(root_ids), std::end( root_ids) );

  for ( const auto root_pair : lazyit::distinct_pairs( root_ids ) ) {

    filename.first = dish2::get_innoculum_filename( root_pair.first );
    filename.second = dish2::get_innoculum_filename( root_pair.second );

    slug.first = dish2::get_innoculum_slug( root_id.first );
    slug.second = dish2::get_innoculum_slug( root_id.second );

    root_id = root_pair;

    phenotype_divergence_detected = dish2::detect_phenotypic_divergence<Spec>(
      dish2::load_innoculum<Spec>( root_id.first ).front(),
      dish2::load_innoculum<Spec>( root_id.second ).front()
    );

    file.Update();

  }

  dish2::log_msg( "dumped interroot_phenotype_differentiation result" );

}

} // namespace dish2

#endif // #ifndef DISH2_RECORD_DUMP_INTERROOT_PHENOTYPE_DIFFERENTIATION_HPP_INCLUDE
