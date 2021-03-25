#pragma once
#ifndef DISH2_RECORD_WRITE_DRAWINGS_HPP_INCLUDE
#define DISH2_RECORD_WRITE_DRAWINGS_HPP_INCLUDE

#include <mutex>

#include "../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "../world/ThreadWorld.hpp"

#include "drawings/DrawerCollection.hpp"

namespace dish2 {

template< typename Spec >
void write_drawings(
  const dish2::ThreadWorld< Spec >& thread_world, const size_t thread_idx
) {
  using polymorphic_drawers_t = typename dish2::DrawerCollection<Spec>::polymorphic_drawers_t;
  // setup drawers as passed in args
  thread_local emp::vector<polymorphic_drawers_t> drawers;

  thread_local std::once_flag make_drawers_once;
  std::call_once(make_drawers_once, [thread_world, thread_idx](){
    emp::vector<std::string> drawing_names;
    emp::slice(cfg.DRAWINGS(), drawing_names, ':');

    dish2::DrawerCollection<Spec> collection( thread_world, thread_idx );

    drawers = collection.MakePolymorphicDrawers(drawing_names);
  });

  for (auto drawer : drawers) {
    drawer.SaveToFile();
  }

  thread_local std::once_flag once_flag;
  std::call_once(once_flag, [thread_idx](){

    std::cout << "proc " << uitsl::get_proc_id() << " thread " << thread_idx
      << " wrote drawings" << std::endl;
  });

}

} // namespace dish2

#endif // #ifndef DISH2_RECORD_WRITE_DRAWINGS_HPP_INCLUDE
