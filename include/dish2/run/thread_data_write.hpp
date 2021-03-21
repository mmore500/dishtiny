#pragma once
#ifndef DISH2_RUN_THREAD_DATA_WRITE_HPP_INCLUDE
#define DISH2_RUN_THREAD_DATA_WRITE_HPP_INCLUDE

#include <limits>

#include "../../../third-party/conduit/include/uitsl/countdown/Timer.hpp"

#include "../config/cfg.hpp"
#include "../record/write_cell_census.hpp"
#include "../record/write_demographic_phenotypic_phylogenetic_metrics.hpp"
#include "../record/write_drawings.hpp"
#include "../record/write_phylogenetic_root_abundances.hpp"
#include "../world/ThreadWorld.hpp"

namespace dish2 {

// data collection tasks that could be run multiple times
template<typename Spec>
void thread_data_write( const dish2::ThreadWorld<Spec>& thread_world ) {

  dish2::write_cell_census<Spec>( thread_world );
  dish2::write_demographic_phenotypic_phylogenetic_metrics<Spec>(thread_world);
  dish2::write_phylogenetic_root_abundances<Spec>( thread_world );

  if ( dish2::cfg.DRAWINGS_WRITE() ) dish2::write_drawings<Spec>(thread_world);

}

} // namespace dish2

#endif // #ifndef DISH2_RUN_THREAD_DATA_WRITE_HPP_INCLUDE
