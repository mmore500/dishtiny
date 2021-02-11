#pragma once
#ifndef DISH2_RECORD_WRITE_DRAWINGS_HPP_INCLUDE
#define DISH2_RECORD_WRITE_DRAWINGS_HPP_INCLUDE

#include <mutex>

#include "../world/ThreadWorld.hpp"

#include "drawings/DrawerCollection.hpp"

namespace dish2 {

template< typename Spec >
void write_drawings(
  const dish2::ThreadWorld< Spec >& thread_world, const size_t thread_idx
) {

  dish2::DrawerCollection<Spec> drawers( thread_world, thread_idx );
  drawers.SaveToFile();

  thread_local std::once_flag once_flag;
  std::call_once(once_flag, [thread_idx](){

    std::cout << "proc " << uitsl::get_proc_id() << " thread " << thread_idx
      << " wrote drawings" << std::endl;
  });

}

} // namespace dish2

#endif // #ifndef DISH2_RECORD_WRITE_DRAWINGS_HPP_INCLUDE
