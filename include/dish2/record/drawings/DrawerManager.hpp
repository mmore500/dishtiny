#pragma once
#ifndef DISH2_RECORD_DRAWINGS_DRAWERMANAGER_HPP_INCLUDE
#define DISH2_RECORD_DRAWINGS_DRAWERMANAGER_HPP_INCLUDE

#include <algorithm>
#include <iterator>
#include <string>
#include <thread>
#include <utility>
#include <variant>

#include "../../../../third-party/Empirical/include/emp/base/array.hpp"
#include "../../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "../../config/thread_idx.hpp"

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

  using polymorphic_drawers_t = PolymorphicDrawer< FirstDrawer, SubsequentDrawers...>;

  constexpr static size_t num_drawers = sizeof...(SubsequentDrawers) + 1;

  emp::array<polymorphic_drawers_t, num_drawers> result_table;

  DrawerManager() = delete;

  DrawerManager(const DrawerManager&) = default;

  template<typename... Args>
  DrawerManager( Args&&... args )
  : first_drawer( std::forward<Args>( args )... )
  , subsequent_drawers( std::forward<Args>( args )... )
  , result_table{
      polymorphic_drawers_t{ std::in_place_type_t<FirstDrawer>{}, std::forward<Args>( args )... },
      polymorphic_drawers_t{ std::in_place_type_t<SubsequentDrawers>{}, std::forward<Args>( args )... }...
    }
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

  emp::vector<polymorphic_drawers_t>
  MakePolymorphicDrawers(const emp::vector<std::string>& names) {
    static constexpr std::string_view lookup[]
      = { FirstDrawer::artist_t::GetName(), SubsequentDrawers::artist_t::GetName()... };

    emp::vector<polymorphic_drawers_t> res;

    std::transform(
      names.begin(),
      names.end(),
      std::back_inserter(res),
      [this](const auto& name) {
        const auto found = std::find_if(
          std::begin( lookup ),
          std::end( lookup ),
          [&name](const auto a) {
            return emp::slugify(std::string{a}) == emp::slugify(name);
          }
        );

        std::cout << "created drawer for " << emp::slugify(name) << std::endl;

        emp_always_assert(
          found != std::end( lookup ), emp::slugify(name)
        );

        const size_t idx = std::distance(
          std::begin( lookup ),
          found
        );
        return result_table[idx];
      }
    );
    return res;
  }
};


} // namespace dish2

#endif // #ifndef DISH2_RECORD_DRAWINGS_DRAWERMANAGER_HPP_INCLUDE
