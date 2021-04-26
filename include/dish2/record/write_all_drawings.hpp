#pragma once
#ifndef DISH2_RECORD_WRITE_ALL_DRAWINGS_HPP_INCLUDE
#define DISH2_RECORD_WRITE_ALL_DRAWINGS_HPP_INCLUDE

#include <chrono>
#include <utility>

#include "../../../third-party/conduit/include/uitsl/countdown/Timer.hpp"

#include "../debug/log_msg.hpp"
#include "../utility/try_with_timeout.hpp"
#include "../world/ThreadWorld.hpp"

#include "drawings/DrawerCollection.hpp"

namespace dish2 {

template< typename Spec >
void write_all_drawings( const dish2::ThreadWorld< Spec >& thread_world ) {

  const uitsl::Timer timer;
  dish2::log_msg(
    "beginning write_all_drawings: ", timer.GetElapsed().count(), "s"
  );

  dish2::DrawerCollection<Spec> drawers( thread_world );

  dish2::log_msg( "constructed drawers: ", timer.GetElapsed().count(), "s" );

  drawers.SaveToFileAsDrawing();

  dish2::log_msg(
    "completed SaveToFileAsDrawing(): ", timer.GetElapsed().count(), "s"
  );

  dish2::log_msg(
    "completed write_all_drawings: ", timer.GetElapsed().count(), "s"
  );

}

} // namespace dish2

#endif // #ifndef DISH2_RECORD_WRITE_ALL_DRAWINGS_HPP_INCLUDE
