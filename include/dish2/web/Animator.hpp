#pragma once
#ifndef DISH2_WEB_ANIMATOR_HPP_INCLUDE
#define DISH2_WEB_ANIMATOR_HPP_INCLUDE

#include <functional>

#include "../../../third-party/Empirical/include/emp/web/Animate.hpp"

namespace dish2 {

class Animator : public emp::web::Animate {

  std::function<void()> update_and_render_callback;

public:

  explicit Animator( std::function<void()> update_and_render_callback_ )
  : update_and_render_callback( update_and_render_callback_ )
  {}

  void DoFrame() override { update_and_render_callback(); }

};

} // namespace dish2

#endif // #ifndef DISH2_WEB_ANIMATOR_HPP_INCLUDE
