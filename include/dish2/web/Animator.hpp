#pragma once
#ifndef DISH2_WEB_ANIMATOR_HPP_INCLUDE
#define DISH2_WEB_ANIMATOR_HPP_INCLUDE

#include <functional>

#include "../../../third-party/Empirical/include/emp/web/Animate.hpp"

namespace dish2 {

class Animator : public emp::web::Animate {

  std::function<void(const bool)> update_and_render_callback;

  bool render_toggle{ true };

public:

  Animator( std::function<void(const bool)> update_and_render_callback_ )
  : update_and_render_callback( update_and_render_callback_ )
  {}

  void ToggleRender() { render_toggle = !render_toggle; }

  void DoFrame() override { update_and_render_callback( render_toggle ); }

};

} // namespace dish2

#endif // #ifndef DISH2_WEB_ANIMATOR_HPP_INCLUDE
