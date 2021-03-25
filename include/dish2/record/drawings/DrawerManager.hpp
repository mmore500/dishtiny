#pragma once
#ifndef DISH2_RECORD_DRAWINGS_DRAWERMANAGER_HPP_INCLUDE
#define DISH2_RECORD_DRAWINGS_DRAWERMANAGER_HPP_INCLUDE

#include <algorithm>
#include <iterator>
#include <string>
#include <thread>
#include <utility>
#include <variant>

#include "../../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../../third-party/Empirical/include/emp/base/array.hpp"

namespace dish2 {

template< typename... Drawers >
class PolymorphicDrawer {

    std::variant<Drawers...> impl;

public:
    PolymorphicDrawer() = delete;

    PolymorphicDrawer(const PolymorphicDrawer&) = default;

    PolymorphicDrawer& operator=(const PolymorphicDrawer&) = default;

    template<typename Drawer, typename... Args>
    PolymorphicDrawer( std::in_place_type_t<Drawer> tag, Args&&... args )
    : impl( tag, std::forward<Args>( args )... )
    { }

    void SaveToFile() {
        // call Draw on the variant impl
        std::visit(
            [&](auto& drawer){ drawer.SaveToFile(); },
            impl
        );
    }
};

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
