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

  emp::vector<std::function<void()>> cbs;

  GridAnimator<int> ga_nodraw;

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

  double min_stockpile;
  GridAnimator<double> ga_stockpiles;

  double min_off_ch_cap;
  GridAnimator<double> ga_off_ch_caps0;
  GridAnimator<double> ga_off_ch_caps1;

  double min_endowment;
  GridAnimator<double> ga_endowments0;
  GridAnimator<double> ga_endowments1;
  GridAnimator<double> ga_endowments2;

  double min_res_pool;
  double max_res_pool;
  GridAnimator<double> ga_res_pools0;
  GridAnimator<double> ga_res_pools1;
  GridAnimator<double> ga_res_pools2;

  double min_avoid_over;
  double max_avoid_over;
  GridAnimator<double> ga_avoid_overs0;
  GridAnimator<double> ga_avoid_overs1;


  Interface()
    : s(100, 0)
    , viewer("emp_viewer")
    , button_dash("emp_button_dash")
    , canvas(500,500)
    , cbs()
    , ga_nodraw(&s, NULL, NULL, NULL, NULL, &cbs)
    , min_resources0(0.0)
    , max_resources0(36.0)
    , ga_resources0(&s, s.gs_resources[0], &min_resources0, &max_resources0, &canvas, &cbs)
    , min_resources1(0.0)
    , max_resources1(36.0)
    , ga_resources1(&s, s.gs_resources[1], &min_resources1, &max_resources1, &canvas, &cbs)
    , min_sig(ACTIVATED)
    , max_sig(QUIESCENT_MAX)
    , ga_signals0(&s, s.gs_signals[0], &min_sig, &max_sig, &canvas, &cbs)
    , ga_signals1(&s, s.gs_signals[1], &min_sig, &max_sig, &canvas, &cbs)
    , min_ch(1)
    , max_ch(CH_MAX)
    , ga_channels0(&s, s.gs_channels[0], &min_ch, &max_ch, &canvas, &cbs)
    , ga_channels1(&s, s.gs_channels[1], &min_ch, &max_ch, &canvas, &cbs)
    , min_stockpile(KILL_THRESH)
    , ga_stockpiles(&s, s.g_stockpiles, &min_stockpile, NULL, &canvas, &cbs)
    , min_off_ch_cap(0.0)
    , ga_off_ch_caps0(&s, s.gs_off_ch_caps[0], &min_off_ch_cap, NULL, &canvas, &cbs)
    , ga_off_ch_caps1(&s, s.gs_off_ch_caps[1], &min_off_ch_cap, NULL, &canvas, &cbs)
    , min_endowment(0.0)
    , ga_endowments0(&s, s.gs_endowments[0], &min_endowment, NULL, &canvas, &cbs)
    , ga_endowments1(&s, s.gs_endowments[1], &min_endowment, NULL, &canvas, &cbs)
    , ga_endowments2(&s, s.gs_endowments[2], &min_endowment, NULL, &canvas, &cbs)
    , min_res_pool(0.0)
    , max_res_pool(1.0)
    , ga_res_pools0(&s, s.gs_res_pools[0], &min_res_pool, &max_res_pool, &canvas, &cbs)
    , ga_res_pools1(&s, s.gs_res_pools[1], &min_res_pool, &max_res_pool, &canvas, &cbs)
    , ga_res_pools2(&s, s.gs_res_pools[2], &min_res_pool, &max_res_pool, &canvas, &cbs)
    , min_avoid_over(0.0)
    , max_avoid_over(1.0)
    , ga_avoid_overs0(&s, s.gs_avoid_overs[0], &min_avoid_over, &max_avoid_over, &canvas, &cbs)
    , ga_avoid_overs1(&s, s.gs_avoid_overs[1], &min_avoid_over, &max_avoid_over, &canvas, &cbs)
  {
    viewer << canvas;
    viewer << "<br />";
    auto ud_text = viewer.AddText("ud_text");
    ud_text << "Update: " << UI::Live(s.cupdate);
    cbs.push_back([this](){ viewer.Text("ud_text").Redraw(); });
    std::cout << typeid(ud_text).name();

    button_dash << ga_nodraw.anim.GetToggleButton("ga_nodraw");
    button_dash << "&nbsp;&nbsp;&nbsp;";
    button_dash << "run without animation" << "<br />";
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
    button_dash << ga_stockpiles.anim.GetToggleButton("ga_stockpiles");
    button_dash << "&nbsp;&nbsp;&nbsp;";
    button_dash << UI::Button([this]() { this->ga_stockpiles.DoFrame(); }, "Step");
    button_dash << "&nbsp;&nbsp;&nbsp;";
    button_dash << "ga_stockpiles" << "<br />";
    button_dash << ga_off_ch_caps0.anim.GetToggleButton("off_ch_caps0");
    button_dash << "&nbsp;&nbsp;&nbsp;";
    button_dash << UI::Button([this]() { this->ga_off_ch_caps0.DoFrame(); }, "Step");
    button_dash << "&nbsp;&nbsp;&nbsp;";
    button_dash << "off_ch_caps0" << "<br />";
    button_dash << ga_off_ch_caps1.anim.GetToggleButton("off_ch_caps1");
    button_dash << "&nbsp;&nbsp;&nbsp;";
    button_dash << UI::Button([this]() { this->ga_off_ch_caps1.DoFrame(); }, "Step");
    button_dash << "&nbsp;&nbsp;&nbsp;";
    button_dash << "off_ch_caps1" << "<br />";
    button_dash << ga_endowments0.anim.GetToggleButton("endowments0");
    button_dash << "&nbsp;&nbsp;&nbsp;";
    button_dash << UI::Button([this]() { this->ga_endowments0.DoFrame(); }, "Step");
    button_dash << "&nbsp;&nbsp;&nbsp;";
    button_dash << "endowments0" << "<br />";
    button_dash << ga_endowments1.anim.GetToggleButton("endowments1");
    button_dash << "&nbsp;&nbsp;&nbsp;";
    button_dash << UI::Button([this]() { this->ga_endowments1.DoFrame(); }, "Step");
    button_dash << "&nbsp;&nbsp;&nbsp;";
    button_dash << "endowments1" << "<br />";
    button_dash << ga_endowments2.anim.GetToggleButton("endowments2");
    button_dash << "&nbsp;&nbsp;&nbsp;";
    button_dash << UI::Button([this]() { this->ga_endowments2.DoFrame(); }, "Step");
    button_dash << "&nbsp;&nbsp;&nbsp;";
    button_dash << "endowments2" << "<br />";
    button_dash << ga_res_pools0.anim.GetToggleButton("res_pools0");
    button_dash << "&nbsp;&nbsp;&nbsp;";
    button_dash << UI::Button([this]() { this->ga_res_pools0.DoFrame(); }, "Step");
    button_dash << "&nbsp;&nbsp;&nbsp;";
    button_dash << "res_pools0" << "<br />";
    button_dash << ga_res_pools1.anim.GetToggleButton("res_pools1");
    button_dash << "&nbsp;&nbsp;&nbsp;";
    button_dash << UI::Button([this]() { this->ga_res_pools1.DoFrame(); }, "Step");
    button_dash << "&nbsp;&nbsp;&nbsp;";
    button_dash << "res_pools1" << "<br />";
    button_dash << ga_res_pools2.anim.GetToggleButton("res_pools2");
    button_dash << "&nbsp;&nbsp;&nbsp;";
    button_dash << UI::Button([this]() { this->ga_res_pools2.DoFrame(); }, "Step");
    button_dash << "&nbsp;&nbsp;&nbsp;";
    button_dash << "res_pools2" << "<br />";
    button_dash << ga_avoid_overs0.anim.GetToggleButton("avoid_overs0");
    button_dash << "&nbsp;&nbsp;&nbsp;";
    button_dash << UI::Button([this]() { this->ga_avoid_overs0.DoFrame(); }, "Step");
    button_dash << "&nbsp;&nbsp;&nbsp;";
    button_dash << "avoid_overs0" << "<br />";
    button_dash << ga_avoid_overs1.anim.GetToggleButton("avoid_overs1");
    button_dash << "&nbsp;&nbsp;&nbsp;";
    button_dash << UI::Button([this]() { this->ga_avoid_overs1.DoFrame(); }, "Step");
    button_dash << "&nbsp;&nbsp;&nbsp;";
    button_dash << "avoid_overs1" << "<br />";
  }

};

Interface interface;

int main()
{
  return 0;
}
