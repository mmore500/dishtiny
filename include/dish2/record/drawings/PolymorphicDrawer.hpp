#pragma once
#ifndef DISH2_RECORD_DRAWINGS_POLYMORPHICDRAWER_HPP_INCLUDE
#define DISH2_RECORD_DRAWINGS_POLYMORPHICDRAWER_HPP_INCLUDE

#include <utility>
#include <variant>

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

  void SaveToFileAsDrawing() {
    // call Draw on the variant impl
    std::visit(
      [&](auto& drawer){ drawer.SaveToFileAsDrawing(); },
      impl
    );
  }

  void SaveToFileAsFrame() {
    // call Draw on the variant impl
    std::visit(
      [&](auto& drawer){ drawer.SaveToFileAsFrame(); },
      impl
    );
  }
};

} // namespace dish2

#endif // #ifndef DISH2_RECORD_DRAWINGS_POLYMORPHICDRAWER_HPP_INCLUDE
