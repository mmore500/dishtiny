#pragma once
#ifndef DISH2_RECORD_WRITE_PHYLOGENETIC_ROOT_ABUNDANCES_HPP_INCLUDE
#define DISH2_RECORD_WRITE_PHYLOGENETIC_ROOT_ABUNDANCES_HPP_INCLUDE

#include <mutex>

#include "../../../third-party/Empirical/include/emp/data/DataFile.hpp"

#include "../config/has_replicate.hpp"
#include "../config/has_series.hpp"
#include "../config/has_stint.hpp"
#include "../introspection/get_root_id_prevalence.hpp"
#include "../introspection/get_unique_root_ids.hpp"

#include "make_filename/make_phylogenetic_root_abundances_filename.hpp"

namespace dish2 {

template< typename Spec >
void write_phylogenetic_root_abundances(
  const dish2::ThreadWorld< Spec >& world, const size_t thread_idx
) {

  thread_local emp::DataFile file(
    dish2::make_phylogenetic_root_abundances_filename(
      thread_idx
    )
  );

  thread_local size_t root_id;
  thread_local double prevalence;
  thread_local size_t update;

  update = world.GetUpdate();

  thread_local std::once_flag once_flag;
  std::call_once(once_flag, [](){
    if ( dish2::has_stint() ) file.AddVal(cfg.STINT(), "Stint");
    if ( dish2::has_series() ) file.AddVal(cfg.SERIES(), "Series");
    if ( dish2::has_replicate() ) file.AddVal(cfg.REPLICATE(), "Replicate");

    file.AddVar(root_id, "Root ID");
    file.AddVar(prevalence, "Prevalence");
    file.AddVar(update, "Update");
    file.PrintHeaderKeys();
  });

  for ( const auto& root_id_ : dish2::get_unique_root_ids<Spec>(world) ) {
    root_id = root_id_;
    prevalence = dish2::get_root_id_prevalence<Spec>( root_id, world );
    file.Update();
  }

}

} // namespace dish2

#endif // #ifndef DISH2_RECORD_WRITE_PHYLOGENETIC_ROOT_ABUNDANCES_HPP_INCLUDE
