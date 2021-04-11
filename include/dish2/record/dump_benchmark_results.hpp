#pragma once
#ifndef DISH2_RECORD_DUMP_BENCHMARK_RESULTS_HPP_INCLUDE
#define DISH2_RECORD_DUMP_BENCHMARK_RESULTS_HPP_INCLUDE

#include <algorithm>
#include <fstream>
#include <string>

#include "../../../third-party/conduit/include/uitsl/algorithm/for_each.hpp"
#include "../../../third-party/conduit/include/uitsl/mpi/comm_utils.hpp"
#include "../../../third-party/Empirical/include/emp/data/DataFile.hpp"
#include "../../../third-party/Empirical/include/emp/tools/keyname_utils.hpp"
#include "../../../third-party/Empirical/include/emp/tools/string_utils.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/utility/CountingIterator.hpp"

#include "../config/cfg.hpp"
#include "../config/has_replicate.hpp"
#include "../config/has_series.hpp"
#include "../config/has_stint.hpp"
#include "../config/thread_idx.hpp"
#include "../debug/log_msg.hpp"
#include "../introspection/sum_state_mesh_intra_attempted_puts.hpp"
#include "../introspection/sum_state_mesh_intra_blocked_puts.hpp"
#include "../introspection/sum_state_mesh_intra_dropped_puts.hpp"
#include "../introspection/sum_state_mesh_intra_net_flux.hpp"
#include "../introspection/sum_state_mesh_intra_read_count.hpp"
#include "../introspection/sum_state_mesh_intra_revision_count.hpp"
#include "../introspection/sum_state_mesh_proc_attempted_puts.hpp"
#include "../introspection/sum_state_mesh_proc_blocked_puts.hpp"
#include "../introspection/sum_state_mesh_proc_dropped_puts.hpp"
#include "../introspection/sum_state_mesh_proc_net_flux.hpp"
#include "../introspection/sum_state_mesh_proc_read_count.hpp"
#include "../introspection/sum_state_mesh_proc_revision_count.hpp"
#include "../introspection/sum_state_mesh_thread_attempted_puts.hpp"
#include "../introspection/sum_state_mesh_thread_blocked_puts.hpp"
#include "../introspection/sum_state_mesh_thread_dropped_puts.hpp"
#include "../introspection/sum_state_mesh_thread_net_flux.hpp"
#include "../introspection/sum_state_mesh_thread_read_count.hpp"
#include "../introspection/sum_state_mesh_thread_revision_count.hpp"
#include "../utility/pare_keyname_filename.hpp"

#include "make_filename/make_benchmark_path.hpp"
#include "make_filename/make_benchmark_results_filename.hpp"

namespace dish2 {

template< typename Spec >
void dump_benchmark_results( const dish2::ThreadWorld< Spec >& world ) {

  const std::string out_filename = dish2::pare_keyname_filename(
    dish2::make_benchmark_results_filename(),
    dish2::make_benchmark_path()
  );

  std::ofstream out_stream( dish2::make_benchmark_path( out_filename ) );
  emp::DataFile file( out_stream );

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

  file.AddVal( dish2::cfg.RUN_SECONDS(), "Run Seconds" );
  file.AddVal( world.GetUpdate(), "Elapsed Updates" );

  // state mesh, intrathread
  file.AddVal(
    dish2::sum_state_mesh_intra_attempted_puts<Spec>( world ),
    "State Mesh Attempted Intrathread Puts"
  );
  file.AddVal(
    dish2::sum_state_mesh_intra_blocked_puts<Spec>( world ),
    "State Mesh Blocked Intrathread Puts"
  );
  file.AddVal(
    dish2::sum_state_mesh_intra_dropped_puts<Spec>( world ),
    "State Mesh Dropped Intrathread Puts"
  );
  file.AddVal(
    dish2::sum_state_mesh_intra_net_flux<Spec>( world ),
    "State Mesh Intrathread Net Flux"
  );
  file.AddVal(
    dish2::sum_state_mesh_intra_read_count<Spec>( world ),
    "State Mesh Intrathread Read Count"
  );
  file.AddVal(
    dish2::sum_state_mesh_intra_revision_count<Spec>( world ),
    "State Mesh Intrathread Revision Count"
  );

  // state mesh, interproc
  file.AddVal(
    dish2::sum_state_mesh_proc_attempted_puts<Spec>( world ),
    "State Mesh Attempted Interprocess Puts"
  );
  file.AddVal(
    dish2::sum_state_mesh_proc_blocked_puts<Spec>( world ),
    "State Mesh Blocked Interprocess Puts"
  );
  file.AddVal(
    dish2::sum_state_mesh_proc_dropped_puts<Spec>( world ),
    "State Mesh Dropped Interprocess Puts"
  );
  file.AddVal(
    dish2::sum_state_mesh_proc_net_flux<Spec>( world ),
    "State Mesh Interprocess Net Flux"
  );
  file.AddVal(
    dish2::sum_state_mesh_proc_read_count<Spec>( world ),
    "State Mesh Interprocess Read Count"
  );
  file.AddVal(
    dish2::sum_state_mesh_proc_revision_count<Spec>( world ),
    "State Mesh Interprocess Revision Count"
  );

  // state mesh, interthread
  file.AddVal(
    dish2::sum_state_mesh_thread_attempted_puts<Spec>( world ),
    "State Mesh Attempted Interthread Puts"
  );
  file.AddVal(
    dish2::sum_state_mesh_thread_blocked_puts<Spec>( world ),
    "State Mesh Blocked Interthread Puts"
  );
  file.AddVal(
    dish2::sum_state_mesh_thread_dropped_puts<Spec>( world ),
    "State Mesh Dropped Interthread Puts"
  );
  file.AddVal(
    dish2::sum_state_mesh_thread_net_flux<Spec>( world ),
    "State Mesh Interthread Net Flux"
  );
  file.AddVal(
    dish2::sum_state_mesh_thread_read_count<Spec>( world ),
    "State Mesh Interthread Read Count"
  );
  file.AddVal(
    dish2::sum_state_mesh_thread_revision_count<Spec>( world ),
    "State Mesh Interthread Revision Count"
  );

  file.PrintHeaderKeys();
  file.Update();

  dish2::log_msg( " dumped benchmark_results log" );

}

} // namespace dish2

#endif // #ifndef DISH2_RECORD_DUMP_BENCHMARK_RESULTS_HPP_INCLUDE
