#pragma once
#ifndef DISH2_RECORD_DUMP_DEATH_LOG_HPP_INCLUDE
#define DISH2_RECORD_DUMP_DEATH_LOG_HPP_INCLUDE

#include <algorithm>

#include "../../../third-party/bxzstr/include/bxzstr.hpp"
#include "../../../third-party/conduit/include/uitsl/algorithm/for_each.hpp"
#include "../../../third-party/Empirical/include/emp/data/DataFile.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/utility/CountingIterator.hpp"

#include "../config/has_replicate.hpp"
#include "../config/has_series.hpp"
#include "../config/has_stint.hpp"

#include "make_filename/make_data_path.hpp"
#include "make_filename/make_death_log_filename.hpp"

namespace dish2 {

template< typename Spec >
void dump_death_log(
  const dish2::ThreadWorld< Spec >& world, const size_t thread_idx
) {

  const auto& population = world.population;

  thread_local bxz::ofstream out_stream(
    dish2::make_data_path( dish2::make_death_log_filename( thread_idx ) ),
    bxz::lzma, 9
  );
  emp::DataFile file( out_stream );

  if ( dish2::has_stint() ) file.AddVal(cfg.STINT(), "Stint");
  if ( dish2::has_series() ) file.AddVal(cfg.SERIES(), "Series");
  if ( dish2::has_replicate() ) file.AddVal(cfg.REPLICATE(), "Replicate");

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

}

} // namespace dish2

#endif // #ifndef DISH2_RECORD_DUMP_DEATH_LOG_HPP_INCLUDE
