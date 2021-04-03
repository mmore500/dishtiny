#pragma once
#ifndef DISH2_RECORD_WRITE_ALL_DRAWINGS_HPP_INCLUDE
#define DISH2_RECORD_WRITE_ALL_DRAWINGS_HPP_INCLUDE

#include <chrono>
#include <utility>

#include "../../../third-party/conduit/include/uitsl/mpi/comm_utils.hpp"
#include "../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "../utility/try_with_timeout.hpp"
#include "../world/ThreadWorld.hpp"

#include "drawings/DrawerCollection.hpp"

namespace dish2 {

template< typename Spec >
void write_all_drawings( const dish2::ThreadWorld< Spec >& thread_world ) {

  dish2::DrawerCollection<Spec> drawers( thread_world );

  using namespace std::chrono_literals;

  if (
    dish2::try_with_timeout( [&](){ drawers.SaveToFileAsDrawing(); }, 5min )
  ) {

    thread_local std::once_flag once_flag;
    std::call_once(once_flag, [](){
      std::cout << "proc " << uitsl::get_proc_id()
        << " thread " << dish2::thread_idx
        << " wrote all drawings" << '\n';
    });

  } else {

    std::cout << "proc " << uitsl::get_proc_id()
      << " thread " << dish2::thread_idx
      << " all drawings write timed out" << '\n';
  }

}

} // namespace dish2

#endif // #ifndef DISH2_RECORD_WRITE_ALL_DRAWINGS_HPP_INCLUDE
