#pragma once
#ifndef DISH2_RECORD_WRITE_ALL_DRAWINGS_HPP_INCLUDE
#define DISH2_RECORD_WRITE_ALL_DRAWINGS_HPP_INCLUDE

#include <chrono>
#include <utility>

#include "../../../third-party/conduit/include/uitsl/mpi/comm_utils.hpp"
#include "../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "../debug/log_msg.hpp"
#include "../utility/try_with_timeout.hpp"
#include "../world/ThreadWorld.hpp"

#include "drawings/DrawerCollection.hpp"

namespace dish2 {

template< typename Spec >
void write_all_drawings( const dish2::ThreadWorld< Spec >& thread_world ) {

  dish2::log_msg( "writing all drawings" );

  dish2::DrawerCollection<Spec> drawers( thread_world );

  using namespace std::chrono_literals;

  if (
    dish2::try_with_timeout( [&](){ drawers.SaveToFileAsDrawing(); }, 5min )
  ) {

    dish2::log_msg( "wrote all drawings" );

  } else {

    dish2::log_msg( "all drawings write timed out" );

  }

}

} // namespace dish2

#endif // #ifndef DISH2_RECORD_WRITE_ALL_DRAWINGS_HPP_INCLUDE
