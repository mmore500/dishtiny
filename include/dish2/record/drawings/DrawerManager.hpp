#pragma once
#ifndef DISH2_RECORD_DRAWINGS_DRAWERMANAGER_HPP_INCLUDE
#define DISH2_RECORD_DRAWINGS_DRAWERMANAGER_HPP_INCLUDE

#include <thread>

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
    std::thread worker( [this](){ first_drawer.SaveToFile(); } );
    subsequent_drawers.SaveToFile();
    worker.join();
  }

};

} // namespace dish2

#endif // #ifndef DISH2_RECORD_DRAWINGS_DRAWERMANAGER_HPP_INCLUDE
