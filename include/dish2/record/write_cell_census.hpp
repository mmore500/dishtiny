#pragma once
#ifndef DISH2_RECORD_WRITE_CELL_CENSUS_HPP_INCLUDE
#define DISH2_RECORD_WRITE_CELL_CENSUS_HPP_INCLUDE

#include <mutex>

#include "../../../third-party/bxzstr/include/bxzstr.hpp"
#include "../../../third-party/Empirical/include/emp/base/macros.hpp"
#include "../../../third-party/Empirical/include/emp/data/DataFile.hpp"

#include "../config/has_replicate.hpp"
#include "../config/has_series.hpp"
#include "../config/has_stint.hpp"

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

  thread_local bxz::ofstream out_stream(
    dish2::make_data_path( dish2::make_cell_census_filename( thread_idx ) ),
    bxz::lzma, 9
  );
  thread_local emp::DataFile file( out_stream );

  thread_local std::string metric;
  thread_local double value;
  thread_local size_t cell_idx;
  thread_local size_t update;

  update = world.GetUpdate();

  thread_local std::once_flag once_flag;
  std::call_once(once_flag, [](){
    if ( dish2::has_stint() ) file.AddVal(cfg.STINT(), "Stint");
    if ( dish2::has_series() ) file.AddVal(cfg.SERIES(), "Series");
    if ( dish2::has_replicate() ) file.AddVal(cfg.REPLICATE(), "Replicate");

    file.AddVar(metric, "Metric");
    file.AddVar(value, "Value");
    file.AddVar(cell_idx, "Cell");
    file.AddVar(update, "Update");
    file.PrintHeaderKeys();
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
