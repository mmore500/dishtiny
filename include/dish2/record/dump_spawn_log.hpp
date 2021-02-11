#pragma once
#ifndef DISH2_RECORD_DUMP_SPAWN_LOG_HPP_INCLUDE
#define DISH2_RECORD_DUMP_SPAWN_LOG_HPP_INCLUDE

#include <algorithm>
#include <string>

#include "../../../third-party/conduit/include/uitsl/algorithm/for_each.hpp"
#include "../../../third-party/Empirical/include/emp/data/DataFile.hpp"
#include "../../../third-party/header-only-gzstream/include/hogzstr/gzstream.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/utility/CountingIterator.hpp"

#include "../config/has_replicate.hpp"
#include "../config/has_series.hpp"
#include "../config/has_stint.hpp"
#include "../utility/pare_keyname_filename.hpp"

#include "make_filename/make_data_path.hpp"
#include "make_filename/make_spawn_log_filename.hpp"

namespace dish2 {

template< typename Spec >
void dump_spawn_log(
  const dish2::ThreadWorld< Spec >& world, const size_t thread_idx
) {

  const auto& population = world.population;

  const thread_local std::string out_filename = dish2::pare_keyname_filename(
    dish2::make_spawn_log_filename( thread_idx ),
    dish2::make_data_path()
  );

  thread_local bxz::ofstream out_stream( dish2::make_data_path(
    out_filename
  ) );
  emp::DataFile file( out_stream );

  if ( dish2::has_stint() ) file.AddVal(cfg.STINT(), "Stint");
  if ( dish2::has_series() ) file.AddVal(cfg.SERIES(), "Series");
  if ( dish2::has_replicate() ) file.AddVal(cfg.REPLICATE(), "Replicate");

  dish2::SpawnEvent<Spec> event;

  file.AddVar(
    event.kin_id_commonality_daughter_eliminated,
    "Daughter-Eliminated Kin ID Commonality"
  );
  file.AddVar(
    event.kin_id_commonality_parent_daughter,
    "Parent-Daughter Kin ID Commonality"
  );
  file.AddVar(
    event.kin_id_commonality_parent_eliminated,
    "Parent-Eliminated Kin ID Commonality"
  );
  uitsl::for_each(
    std::begin( event.peripherality_parent ),
    std::end( event.peripherality_parent ),
    sgpl::CountingIterator{},
    [&]( const auto& val, const size_t lev ){ file.AddVar(
      val, emp::to_string("Parent Cell Level ", lev, " Peripherality")
    ); }
  );
  file.AddVar(event.replev, "Replev");

  file.PrintHeaderKeys();

  std::for_each(
    std::begin( population ), std::end( population ),
    [&]( const auto& cell ){
      const auto& log
        = cell.running_logs.template GetLog<dish2::SpawnEvent<Spec>>();
      std::for_each(
        std::begin( log ), std::end( log ),
        [&]( const auto& spawn_event ){
          event = spawn_event;
          file.Update();
        }
      );
    }
  );

  std::cout << "proc " << uitsl::get_proc_id() << " thread " << thread_idx
    << " dumped spawn log" << std::endl;

}

} // namespace dish2

#endif // #ifndef DISH2_RECORD_DUMP_SPAWN_LOG_HPP_INCLUDE
