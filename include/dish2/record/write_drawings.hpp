#pragma once
#ifndef DISH2_RECORD_WRITE_DRAWINGS_HPP_INCLUDE
#define DISH2_RECORD_WRITE_DRAWINGS_HPP_INCLUDE

#include "../world/ThreadWorld.hpp"

#include "drawings/DrawerCollection.hpp"

namespace dish2 {

template< typename Spec >
void write_drawings(
  const dish2::ThreadWorld< Spec >& thread_world, const size_t thread_idx
) {

  dish2::DrawerCollection<Spec> drawers( thread_world, thread_idx );
  drawers.SaveToFile();

}

} // namespace dish2

#endif // #ifndef DISH2_RECORD_WRITE_DRAWINGS_HPP_INCLUDE
