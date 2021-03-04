#pragma once
#ifndef DISH2_RECORD_WRITE_CELL_CENSUS_HPP_INCLUDE
#define DISH2_RECORD_WRITE_CELL_CENSUS_HPP_INCLUDE

#include <mutex>
#include <string>

#include "../../../third-party/bxzstr/include/bxzstr.hpp"
#include "../../../third-party/conduit/include/uitsl/mpi/comm_utils.hpp"
#include "../../../third-party/Empirical/include/emp/base/macros.hpp"
#include "../../../third-party/Empirical/include/emp/data/DataFile.hpp"
#include "../../../third-party/Empirical/include/emp/tools/keyname_utils.hpp"
#include "../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "../config/has_replicate.hpp"
#include "../config/has_series.hpp"
#include "../config/has_stint.hpp"
#include "../utility/pare_keyname_filename.hpp"

#include "cell_census/write_cell_age.hpp"
#include "cell_census/write_kin_group_age.hpp"
#include "cell_census/write_kin_group_id.hpp"
#include "cell_census/write_peripheral_count.hpp"
#include "cell_census/write_resource_stockpile.hpp"
#include "cell_census/write_spawn_count.hpp"
#include "make_filename/make_cell_census_filename.hpp"
#include "make_filename/make_data_path.hpp"

namespace dish2 {

template< typename Spec >
void write_cell_census(
  const dish2::ThreadWorld< Spec >& world, const size_t thread_idx
) {

  const thread_local std::string out_filename = dish2::pare_keyname_filename(
    dish2::make_cell_census_filename( thread_idx ),
    dish2::make_data_path()
  );

  thread_local bxz::ofstream out_stream(
    dish2::make_data_path( out_filename ), bxz::lzma, 9
  );
  thread_local emp::DataFile file( out_stream );

  thread_local std::string metric;
  thread_local double value;
  thread_local size_t cell_idx;
  thread_local size_t update;

  update = world.GetUpdate();

  thread_local std::once_flag once_flag;
  std::call_once(once_flag, [thread_idx](){
    if ( dish2::has_stint() ) file.AddVal(cfg.STINT(), "Stint");
    if ( dish2::has_series() ) file.AddVal(cfg.SERIES(), "Series");
    if ( dish2::has_replicate() ) file.AddVal(cfg.REPLICATE(), "Replicate");
    file.AddVal(cfg.TREATMENT(), "Treatment");
    for ( const auto& [k, v] : emp::keyname::unpack( cfg.TREATMENT() ) ) {
      file.AddVal( emp::to_string("Treatment ", k), v );
    }
    file.AddVal( "proc", emp::to_string( uitsl::get_proc_id() ) );
    file.AddVal( "thread", emp::to_string( thread_idx ) );

    file.AddVar(metric, "Metric");
    file.AddVar(value, "Value");
    file.AddVar(cell_idx, "Cell");
    file.AddVar(update, "Update");
    file.PrintHeaderKeys();

    std::cout << "proc " << uitsl::get_proc_id() << " thread " << thread_idx
      << " wrote cell census" << std::endl;
  });


  dish2::write_cell_age<Spec>( world, file, metric, value, cell_idx );
  dish2::write_kin_group_age<Spec>( world, file, metric, value, cell_idx );
  dish2::write_kin_group_id<Spec>( world, file, metric, value, cell_idx );
  dish2::write_peripheral_count<Spec>( world, file, metric, value, cell_idx );
  dish2::write_resource_stockpile<Spec>( world, file, metric, value, cell_idx );
  dish2::write_spawn_count<Spec>( world, file, metric, value, cell_idx );

}

} // namespace dish2

#endif // #ifndef DISH2_RECORD_WRITE_CELL_CENSUS_HPP_INCLUDE
