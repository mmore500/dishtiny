#include "web/web.h"
#include "base/vector.h"

#include "../Simulation.h"
#include "../GridAnimator.h"

namespace UI = emp::web;

struct Interface {
  Simulation s;

  UI::Document viewer;        //< Div that contains the canvas viewer.
  UI::Document button_dash;  //< Div that contains the button dashboard.
  UI::Canvas canvas;

  double min_resources0;
  double max_resources0;
  GridAnimator<double> ga_resources0;

  double min_resources1;
  double max_resources1;
  GridAnimator<double> ga_resources1;

  int min_sig;
  int max_sig;
  GridAnimator<int> ga_signals0;
  GridAnimator<int> ga_signals1;

  int min_ch;
  int max_ch;
  GridAnimator<int> ga_channels0;
  GridAnimator<int> ga_channels1;

  GridAnimator<double> ga_stockpiles0;
  GridAnimator<double> ga_stockpiles1;

  Interface()
    : s(100, 0)
    , viewer("emp_viewer")
    , button_dash("emp_button_dash")
    , canvas(500,500)
    , min_resources0(0.0)
    , max_resources0(36.0)
    , ga_resources0(s, s.gs_resources[0], &min_resources0, &max_resources0, canvas)
    , min_resources1(0.0)
    , max_resources1(36.0)
    , ga_resources1(s, s.gs_resources[1], &min_resources1, &max_resources1, canvas)
    , min_sig(ACTIVATED)
    , max_sig(QUIESCENT_MAX)
    , ga_signals0(s, s.gs_signals[0], &min_sig, &max_sig, canvas)
    , ga_signals1(s, s.gs_signals[1], &min_sig, &max_sig, canvas)
    , min_ch(0)
    , max_ch(CH_MAX)
    , ga_channels0(s, s.gs_channels[0], &min_ch, &max_ch, canvas)
    , ga_channels1(s, s.gs_channels[1], &min_ch, &max_ch, canvas)
    , ga_stockpiles0(s, s.gs_stockpiles[0], NULL, NULL, canvas)
    , ga_stockpiles1(s, s.gs_stockpiles[1], NULL, NULL, canvas)
  {
    viewer << canvas;
    button_dash << ga_resources0.anim.GetToggleButton("ga_resources0");
    button_dash << "&nbsp;&nbsp;&nbsp;";
    button_dash << UI::Button([this]() { this->ga_resources0.DoFrame(); }, "Step");
    button_dash << "&nbsp;&nbsp;&nbsp;";
    button_dash << "ga_resources0" << "<br />";
    button_dash << ga_resources1.anim.GetToggleButton("ga_resources1");
    button_dash << "&nbsp;&nbsp;&nbsp;";
    button_dash << UI::Button([this]() { this->ga_resources1.DoFrame(); }, "Step");
    button_dash << "&nbsp;&nbsp;&nbsp;";
    button_dash << "ga_resources1" << "<br />";
    button_dash << ga_signals0.anim.GetToggleButton("ga_signals0");
    button_dash << "&nbsp;&nbsp;&nbsp;";
    button_dash << UI::Button([this]() { this->ga_signals0.DoFrame(); }, "Step");
    button_dash << "&nbsp;&nbsp;&nbsp;";
    button_dash << "ga_signals0" << "<br />";
    button_dash << ga_signals1.anim.GetToggleButton("ga_signals1");
    button_dash << "&nbsp;&nbsp;&nbsp;";
    button_dash << UI::Button([this]() { this->ga_signals1.DoFrame(); }, "Step");
    button_dash << "&nbsp;&nbsp;&nbsp;";
    button_dash << "ga_signals1" << "<br />";
    button_dash << ga_channels0.anim.GetToggleButton("ga_channels0");
    button_dash << "&nbsp;&nbsp;&nbsp;";
    button_dash << UI::Button([this]() { this->ga_channels0.DoFrame(); }, "Step");
    button_dash << "&nbsp;&nbsp;&nbsp;";
    button_dash << "ga_channels0" << "<br />";
    button_dash << ga_channels1.anim.GetToggleButton("ga_channels1");
    button_dash << "&nbsp;&nbsp;&nbsp;";
    button_dash << UI::Button([this]() { this->ga_channels1.DoFrame(); }, "Step");
    button_dash << "&nbsp;&nbsp;&nbsp;";
    button_dash << "ga_channels1" << "<br />";
    button_dash << ga_stockpiles0.anim.GetToggleButton("ga_stockpiles0");
    button_dash << "&nbsp;&nbsp;&nbsp;";
    button_dash << UI::Button([this]() { this->ga_stockpiles0.DoFrame(); }, "Step");
    button_dash << "&nbsp;&nbsp;&nbsp;";
    button_dash << "ga_stockpiles0" << "<br />";
    button_dash << ga_stockpiles1.anim.GetToggleButton("ga_stockpiles1");
    button_dash << "&nbsp;&nbsp;&nbsp;";
    button_dash << UI::Button([this]() { this->ga_stockpiles1.DoFrame(); }, "Step");
    button_dash << "&nbsp;&nbsp;&nbsp;";
    button_dash << "ga_stockpiles1" << "<br />";
  }

};

Interface interface;

int main()
{
  return 0;
}
