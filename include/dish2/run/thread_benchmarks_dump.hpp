#pragma once
#ifndef DISH2_RUN_THREAD_BENCHMARKS_DUMP_HPP_INCLUDE
#define DISH2_RUN_THREAD_BENCHMARKS_DUMP_HPP_INCLUDE

#include <fstream>

#include "../record/dump_benchmark_results.hpp"
#include "../record/make_filename/make_benchmark_path.hpp"
#include "../record/make_filename/make_elapsed_updates_filename.hpp"
#include "../utility/pare_keyname_filename.hpp"
#include "../world/ThreadWorld.hpp"

namespace dish2 {

// data collection tasks that must be run only once
template<typename Spec>
void thread_benchmarks_dump( const dish2::ThreadWorld<Spec>& thread_world ) {

  const std::string elapsed_updates_filename = dish2::pare_keyname_filename(
    dish2::make_elapsed_updates_filename(),
    dish2::make_benchmark_path()
  );
  std::ofstream(
    dish2::make_benchmark_path( elapsed_updates_filename )
  ) << thread_world.GetUpdate() << '\n';



  dish2::dump_benchmark_results<Spec>( thread_world );

}

} // namespace dish2

#endif // #ifndef DISH2_RUN_THREAD_BENCHMARKS_DUMP_HPP_INCLUDE
