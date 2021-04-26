#pragma once
#ifndef DISH2_RECORD_DRAWINGS_DRAWERMANAGER_HPP_INCLUDE
#define DISH2_RECORD_DRAWINGS_DRAWERMANAGER_HPP_INCLUDE

#include <algorithm>
#include <iterator>
#include <string>
#include <thread>
#include <utility>

#include "../../../../third-party/Empirical/include/emp/base/array.hpp"
#include "../../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "../../debug/log_msg.hpp"

#include "PolymorphicDrawer.hpp"

namespace dish2 {

// base case
template<typename... SubsequentDrawers> struct DrawerManager {

  template<typename... Args> DrawerManager( Args&&... args ) {}

  void SaveToFileAsDrawing() {}

  void SaveToFileAsFrame() {}

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

  void SaveToFileAsDrawing() {
    first_drawer.SaveToFileAsDrawing();
    subsequent_drawers.SaveToFileAsDrawing();
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

        dish2::log_msg( "created drawer for ", emp::slugify(name) );

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
