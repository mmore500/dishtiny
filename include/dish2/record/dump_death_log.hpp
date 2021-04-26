#pragma once
#ifndef DISH2_RECORD_DUMP_DEATH_LOG_HPP_INCLUDE
#define DISH2_RECORD_DUMP_DEATH_LOG_HPP_INCLUDE

#include <algorithm>
#include <string>

#include "../../../third-party/bxzstr/include/bxzstr.hpp"
#include "../../../third-party/conduit/include/uitsl/algorithm/for_each.hpp"
#include "../../../third-party/Empirical/include/emp/data/DataFile.hpp"
#include "../../../third-party/Empirical/include/emp/tools/keyname_utils.hpp"
#include "../../../third-party/Empirical/include/emp/tools/string_utils.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/utility/CountingIterator.hpp"

#include "../config/has_replicate.hpp"
#include "../config/has_series.hpp"
#include "../config/has_stint.hpp"
#include "../debug/log_msg.hpp"
#include "../utility/pare_keyname_filename.hpp"

#include "make_filename/make_data_path.hpp"
#include "make_filename/make_death_log_filename.hpp"

namespace dish2 {

template< typename Spec >
void dump_death_log( const dish2::ThreadWorld< Spec >& world ) {

  const auto& population = world.population;

  const thread_local std::string out_filename = dish2::pare_keyname_filename(
    dish2::make_death_log_filename(),
    dish2::make_data_path()
  );

  thread_local bxz::ofstream out_stream(
    dish2::make_data_path( out_filename ), bxz::lzma, 6
  );
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

  dish2::DeathEvent<Spec> event;

  file.AddVar(event.cause_of_death, "Cause of Death");
  uitsl::for_each(
    std::begin( event.peripherality_deceased ),
    std::end( event.peripherality_deceased ),
    sgpl::CountingIterator{},
    [&]( const auto& val, const size_t lev ){ file.AddVar(
      val, emp::to_string("Deceased Cell Level ", lev, " Peripherality")
    ); }
  );

  file.PrintHeaderKeys();

  std::for_each(
    std::begin( population ), std::end( population ),
    [&]( const auto& cell ){
      const auto& log = cell.running_logs.template GetLog<
        dish2::DeathEvent<Spec>
      >();
      std::for_each(
        std::begin( log ), std::end( log ),
        [&]( const auto& death_event ){
          event = death_event;
          file.Update();
        }
      );
    }
  );

  dish2::log_msg( "dumped death log" );

}

} // namespace dish2

#endif // #ifndef DISH2_RECORD_DUMP_DEATH_LOG_HPP_INCLUDE
