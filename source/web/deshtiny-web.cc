#include "web/web.h"
#include "base/vector.h"

#include "../Simulation.h"
#include "../GridAnimator.h"

namespace UI = emp::web;

struct Interface {
  Simulation s;

  UI::Document doc;
  UI::Canvas canvas;

  double min_resources0;
  double max_resources0;
  GridAnimator<double> ga_resources0;

  double min_resources1;
  double max_resources1;
  GridAnimator<double> ga_resources1;

  GridAnimator<bool> ga_taps0;
  GridAnimator<bool> ga_taps1;

  Interface()
    : s(100, 0)
    , doc("emp_base")
    , canvas(400,400)
    , min_resources0(0.0)
    , max_resources0(36.0)
    , ga_resources0(s, s.gs_resources[0], &min_resources0, &max_resources0, canvas)
    , min_resources1(0.0)
    , max_resources1(36.0)
    , ga_resources1(s, s.gs_resources[1], &min_resources1, &max_resources1, canvas)
    , ga_taps0(s, s.gs_taps[0], NULL, NULL, canvas)
    , ga_taps1(s, s.gs_taps[1], NULL, NULL, canvas)
  {
    doc << canvas;
    doc << ga_resources0.anim.GetToggleButton("ga_resources0");
    doc << ga_resources1.anim.GetToggleButton("ga_resources1");
    doc << ga_taps0.anim.GetToggleButton("ga_taps0");
    doc << ga_taps1.anim.GetToggleButton("ga_taps1");
  }

};

Interface interface;

int main()
{
  return 0;
}
