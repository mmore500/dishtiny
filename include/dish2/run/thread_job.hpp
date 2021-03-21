#pragma once
#ifndef DISH2_RUN_THREAD_JOB_HPP_INCLUDE
#define DISH2_RUN_THREAD_JOB_HPP_INCLUDE

#include <fstream>

#include "../../../third-party/conduit/include/uitsl/mpi/comm_utils.hpp"

#include "../config/cfg.hpp"
#include "../config/thread_idx.hpp"
#include "../load/load_world.hpp"
#include "../record/dump_interroot_phenotype_differentiation.hpp"
#include "../record/make_filename/make_elapsed_updates_filename.hpp"
#include "../world/ThreadWorld.hpp"

#include "thread_artifacts_dump.hpp"
#include "thread_data_dump.hpp"
#include "thread_data_write.hpp"
#include "thread_evolve.hpp"

namespace dish2 {

template<typename Spec>
void thread_job( dish2::ThreadWorld<Spec> thread_world ) {

  dish2::load_world<Spec>( thread_world );

  if ( cfg.RUN() ) dish2::thread_evolve<Spec>( thread_world );

  // write elapsed updates to file (for easier benchmark post-processing)
  if (cfg.BENCHMARKING_DUMP() ) std::ofstream(
    dish2::make_elapsed_updates_filename()
  ) << thread_world.GetUpdate() << std::endl;

  if ( cfg.ARTIFACTS_DUMP() ) {
    dish2::thread_artifacts_dump<Spec>( thread_world );
    std::cout << "proc " << uitsl::get_proc_id()
      << " thread " << dish2::thread_idx
      << " artifacts dump complete" << std::endl;
  }

  if (dish2::cfg.GENESIS() == "innoculate") {
    dish2::dump_coalescence_result<Spec>( thread_world );
  }

  if ( cfg.DATA_DUMP() ) {
    dish2::thread_data_dump<Spec>( thread_world );
    std::cout << "proc " << uitsl::get_proc_id()
      << " thread " << dish2::thread_idx
      << " data dump complete" << std::endl;

    dish2::thread_data_write<Spec>( thread_world );
    std::cout << "proc " << uitsl::get_proc_id()
      << " thread " << dish2::thread_idx
      << " write 0" << std::endl;
    thread_world.Update();
    std::cout << "proc " << uitsl::get_proc_id()
      << " thread " << dish2::thread_idx
      << " update step" << std::endl;
    dish2::thread_data_write<Spec>( thread_world );
    std::cout << "proc " << uitsl::get_proc_id()
      << " thread " << dish2::thread_idx
      << " write 1" << std::endl;
  }

  if ( cfg.TEST_INTERROOT_PHENOTYPE_DIFFERENTIATION() ) {
    dish2::dump_interroot_phenotype_differentiation( thread_world );
  }

  std::cout << "proc " << uitsl::get_proc_id()
    << " thread " << dish2::thread_idx
    << " thread job complete" << std::endl;

}

} // namespace dish2

#endif // #ifndef DISH2_RUN_THREAD_JOB_HPP_INCLUDE
