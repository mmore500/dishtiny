#pragma once
#ifndef DISH2_RECORD_DUMP_COALESCENCE_RESULT_HPP_INCLUDE
#define DISH2_RECORD_DUMP_COALESCENCE_RESULT_HPP_INCLUDE

#include <fstream>
#include <string>

#include "../../../third-party/conduit/include/uitsl/meta/function_cast.hpp"
#include "../../../third-party/conduit/include/uitsl/mpi/comm_utils.hpp"
#include "../../../third-party/conduit/include/uitsl/utility/keyname_key_union.hpp"
#include "../../../third-party/Empirical/include/emp/data/DataFile.hpp"
#include "../../../third-party/Empirical/include/emp/tools/keyname_utils.hpp"
#include "../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "../config/get_endeavor.hpp"
#include "../config/get_repro.hpp"
#include "../config/has_replicate.hpp"
#include "../config/has_series.hpp"
#include "../config/has_stint.hpp"
#include "../introspection/get_root_id_prevalence.hpp"
#include "../load/count_root_ids.hpp"
#include "../load/get_innoculum_filename.hpp"
#include "../load/get_innoculum_filenames.hpp"
#include "../load/get_innoculum_slug.hpp"
#include "../load/get_root_ids.hpp"
#include "../utility/calc_fitness_differential.hpp"
#include "../world/ThreadWorld.hpp"

#include "make_filename/make_coalescence_result_filename.hpp"
#include "make_filename/make_data_path.hpp"

namespace dish2 {

template< typename Spec >
void dump_coalescence_result(
  const dish2::ThreadWorld< Spec >& world, const size_t thread_idx
) {

  emp::DataFile file(
    dish2::make_data_path( dish2::make_coalescence_result_filename(
      thread_idx
    ) )
  );

  if ( dish2::has_stint() ) file.AddVal(cfg.STINT(), "Competition Stint");
  if ( dish2::has_series() ) file.AddVal(cfg.SERIES(), "Competition Series");
  if ( dish2::has_replicate() ) file.AddVal(
    cfg.REPLICATE(), "Competition Replicate"
  );
  if ( dish2::get_endeavor() ) file.AddVal(
    *dish2::get_endeavor(), "Competition Endeavor"
  );
  if ( dish2::get_repro() ) file.AddVal(
    *dish2::get_repro(), "Competition Repro"
  );
  file.AddVal( thread_idx, "Competition Thread" );
  file.AddVal( uitsl::get_proc_id(), "Competition Process" );

  const size_t population_size = world.GetSize();
  const size_t update = world.GetUpdate();

  std::string filename;
  std::string slug;
  size_t root_id;
  double fitness_differential;
  double prevalence;

  file.AddVar(population_size, "Population Size");
  file.AddVar(prevalence, "Prevalence");
  file.AddVar(root_id, "Root ID");
  file.AddVar(slug, "Genome Slug");
  file.AddVar(filename, "Genome Filename");
  file.AddVar(update, "Update");
  file.AddVar(fitness_differential, "Fitness Differential");

  for (
    const auto& key
    : uitsl::keyname_key_union( dish2::get_innoculum_filenames() )
  ) if (key != "_" ) file.AddFun( uitsl::function_cast( [key, &filename](){
    // unpacks /current/ filename for specified key
    const auto attrs = emp::keyname::unpack( filename );
    const auto res = attrs.find( key );
    return res != std::end( attrs ) ? res->second : "";
  } ), emp::to_string("genome ", key) );

  file.PrintHeaderKeys();

  for ( const size_t root_id_ : dish2::get_root_ids() ) {

    filename = dish2::get_innoculum_filename( root_id_ );
    slug = dish2::get_innoculum_slug( root_id_ );
    root_id = root_id_;
    prevalence =  dish2::get_root_id_prevalence< Spec >( root_id_, world );

    fitness_differential = dish2::calc_fitness_differential(
      update, prevalence, dish2::count_root_ids()
    );

    file.Update();

  }


}

} // namespace dish2

#endif // #ifndef DISH2_RECORD_DUMP_COALESCENCE_RESULT_HPP_INCLUDE
