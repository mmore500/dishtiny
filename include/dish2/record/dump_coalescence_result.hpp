#pragma once
#ifndef DISH2_RECORD_DUMP_COALESCENCE_RESULT_HPP_INCLUDE
#define DISH2_RECORD_DUMP_COALESCENCE_RESULT_HPP_INCLUDE

#include <fstream>
#include <string>

#include "../../../third-party/Empirical/include/emp/data/DataFile.hpp"

#include "../introspection/get_root_id_prevalence.hpp"
#include "../world/ThreadWorld.hpp"

#include "make_filename/make_coalescence_result_filename.hpp"

namespace dish2 {

template< typename Spec >
void dump_coalescence_result(
  const dish2::ThreadWorld< Spec >& world, const size_t thread_idx
) {

  thread_local emp::DataFile file(
    dish2::make_coalescence_result_filename(
      thread_idx
    )
  );

  const size_t root_id = 0;
  const size_t population_size = world.GetSize();
  const double prevalence = dish2::get_root_id_prevalence< Spec >( 0, world );
  const size_t update = world.GetUpdate();

  file.AddVar(population_size, "Population Size");
  file.AddVar(prevalence, "Prevalence");
  file.AddVar(root_id, "Root ID");
  file.AddVar(update, "Update");

  file.PrintHeaderKeys();
  file.Update();

}

} // namespace dish2

#endif // #ifndef DISH2_RECORD_DUMP_COALESCENCE_RESULT_HPP_INCLUDE
