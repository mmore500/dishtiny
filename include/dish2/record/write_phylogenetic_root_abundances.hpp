#pragma once
#ifndef DISH2_RECORD_WRITE_PHYLOGENETIC_ROOT_ABUNDANCES_HPP_INCLUDE
#define DISH2_RECORD_WRITE_PHYLOGENETIC_ROOT_ABUNDANCES_HPP_INCLUDE

#include <mutex>
#include <string>

#include "../../../third-party/conduit/include/uitsl/mpi/comm_utils.hpp"
#include "../../../third-party/Empirical/include/emp/data/DataFile.hpp"
#include "../../../third-party/Empirical/include/emp/tools/keyname_utils.hpp"
#include "../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "../config/get_endeavor.hpp"
#include "../config/get_repro.hpp"
#include "../config/get_slurm_job_id.hpp"
#include "../config/has_replicate.hpp"
#include "../config/has_series.hpp"
#include "../config/has_stint.hpp"
#include "../config/thread_idx.hpp"
#include "../debug/log_msg.hpp"
#include "../introspection/get_root_id_abundance.hpp"
#include "../introspection/get_root_id_prevalence.hpp"
#include "../introspection/get_unique_root_ids.hpp"
#include "../utility/pare_keyname_filename.hpp"

#include "make_filename/make_data_path.hpp"
#include "make_filename/make_phylogenetic_root_abundances_filename.hpp"

namespace dish2 {

template< typename Spec >
void write_phylogenetic_root_abundances(
  const dish2::ThreadWorld< Spec >& world
) {

  dish2::log_msg( "writing phylogenetic root abundances" );

  const thread_local std::string out_filename = dish2::pare_keyname_filename(
    dish2::make_phylogenetic_root_abundances_filename(),
    dish2::make_data_path()
  );

  thread_local emp::DataFile file( dish2::make_data_path( out_filename ) );

  thread_local size_t root_id;
  thread_local double abundance;
  thread_local size_t count;
  thread_local double prevalence;
  thread_local size_t update;

  update = world.GetUpdate();

  thread_local std::once_flag once_flag;
  std::call_once(once_flag, [num_slots=world.GetSize()](){
    if ( dish2::has_stint() ) file.AddVal(cfg.STINT(), "Stint");
    if ( dish2::has_series() ) file.AddVal(cfg.SERIES(), "Series");
    if ( dish2::has_replicate() ) file.AddVal(cfg.REPLICATE(), "Replicate");
    file.AddVal(cfg.TREATMENT(), "Treatment");
    if ( cfg.TREATMENT().find('=') != std::string::npos ) {
      for ( const auto& [k, v] : emp::keyname::unpack( cfg.TREATMENT() ) ) {
        file.AddVal( v, emp::to_string("Treatment ", k) );
      }
    }
    if ( dish2::get_endeavor() ) file.AddVal(*dish2::get_endeavor(), "Endeavor"
    );
    if ( dish2::get_repro() ) file.AddVal(
      *dish2::get_repro(), "Repro"
    );
    if ( dish2::get_slurm_job_id() ) file.AddVal(
      *dish2::get_slurm_job_id(), "Slurm Job ID"
    );

    file.AddVal( uitsl::get_proc_id(), "proc" );
    file.AddVal( dish2::thread_idx, "thread" );

    file.AddVal(num_slots, "Population Slots");
    file.AddVar(root_id, "Root ID");
    file.AddVar(abundance, "Abundance", "Proportion of available slots.");
    file.AddVar(count, "Count");
    file.AddVar(prevalence, "Prevalence", "Proportion of live cells.");
    file.AddVar(update, "Update");

    file.PrintHeaderKeys();

  });

  for ( const auto& root_id_ : dish2::get_unique_root_ids<Spec>(world) ) {
    root_id = root_id_;
    abundance = dish2::get_root_id_abundance<Spec>( root_id, world );
    count = dish2::get_root_id_count<Spec>( root_id, world );
    prevalence = dish2::get_root_id_prevalence<Spec>( root_id, world );
    file.Update();
  }

  dish2::log_msg( "wrote phylogenetic root abundances" );

}

} // namespace dish2

#endif // #ifndef DISH2_RECORD_WRITE_PHYLOGENETIC_ROOT_ABUNDANCES_HPP_INCLUDE
