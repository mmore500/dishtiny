#pragma once
#ifndef DISH2_RECORD_DRAWINGS_DRAWERMANAGER_HPP_INCLUDE
#define DISH2_RECORD_DRAWINGS_DRAWERMANAGER_HPP_INCLUDE

#include <thread>

#include "../../config/thread_idx.hpp"

namespace dish2 {

// base case
template<typename... SubsequentDrawers> struct DrawerManager {

  template<typename... Args> DrawerManager( Args&&... args ) {}

  void SaveToFile() {}

};

// adapted from https://stackoverflow.com/a/35284581
template<typename FirstDrawer, typename... SubsequentDrawers>
struct DrawerManager<FirstDrawer, SubsequentDrawers...> {

  using subsequent_drawers_t = dish2::DrawerManager<SubsequentDrawers...>;

  FirstDrawer first_drawer;
  DrawerManager<SubsequentDrawers...> subsequent_drawers;

  template<typename... Args>
  DrawerManager( Args&&... args )
  : first_drawer( std::forward<Args>( args )... )
  , subsequent_drawers( std::forward<Args>( args )... )
  {}

  void SaveToFile() {
    const auto parent_thread_idx = dish2::thread_idx;
    std::thread worker( [this, parent_thread_idx](){
      dish2::thread_idx = parent_thread_idx;
      first_drawer.SaveToFile();
    } );
    subsequent_drawers.SaveToFile();
    worker.join();
  }

};

} // namespace dish2

#endif // #ifndef DISH2_RECORD_DRAWINGS_DRAWERMANAGER_HPP_INCLUDE
