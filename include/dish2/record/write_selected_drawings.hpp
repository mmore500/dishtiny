#pragma once
#ifndef DISH2_RECORD_WRITE_SELECTED_DRAWINGS_HPP_INCLUDE
#define DISH2_RECORD_WRITE_SELECTED_DRAWINGS_HPP_INCLUDE

#include <chrono>
#include <future>
#include <mutex>
#include <utility>

#include "../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "../config/thread_idx.hpp"
#include "../debug/log_msg.hpp"
#include "../utility/try_with_timeout.hpp"
#include "../world/ThreadWorld.hpp"

#include "drawings/DrawerCollection.hpp"

namespace dish2 {

template< typename Spec >
void write_selected_drawings( const dish2::ThreadWorld< Spec >& thread_world ) {

  dish2::log_msg( "writing selected drawings" );

  using polymorphic_drawers_t = typename dish2::DrawerCollection<Spec>::polymorphic_drawers_t;

  thread_local emp::vector<polymorphic_drawers_t> drawers = [&thread_world](){

    emp::vector<std::string> drawing_names;
    emp::slice(cfg.SELECTED_DRAWINGS(), drawing_names, ':');

    dish2::DrawerCollection<Spec> collection( thread_world );

    return collection.MakePolymorphicDrawers(drawing_names);

  }();

  // otherwise, packaged task gets its own thread_local drawers
  auto& target_drawers = drawers;

  // drawings occasionally hang, so add a time out
  using namespace std::chrono_literals;
  const bool success = dish2::try_with_timeout(
    [&](){ for (auto& drawer : target_drawers) drawer.SaveToFileAsDrawing(); },
    5min
  );

  if ( success ) {
    thread_local std::once_flag once_flag;
    std::call_once(once_flag, [](){
      dish2::log_msg( "wrote selected drawings" );
    });
  } else {
    dish2::log_msg( "selected drawings write timed out" );
  }

}

} // namespace dish2

#endif // #ifndef DISH2_RECORD_WRITE_SELECTED_DRAWINGS_HPP_INCLUDE
