#pragma once

#include <algorithm>
#include <optional>
#include <string>

#include "base/vector.h"
#include "web/Button.h"
#include "web/Input.h"
#include "web/web.h"
#include "web/Animate.h"
#include "tools/math.h"
#include "tools/Random.h"
#include "tools/keyname_utils.h"

#include "Config.h"
#include "DishWorld.h"
#include "Genome.h"
#include "WebArtist.h"

namespace UI = emp::web;

class WebInterface : public UI::Animate {

  const Config &cfg;

  UI::Document button_dash;  //< Div that contains the button dashboard.
  UI::Document systematics_dash;  //< Div that contains the systematics info.
  UI::Document dominant_viewer;

  DishWorld w;

  UI::Document grid_viewer;
  UI::Document view_selector;
  emp::vector<emp::vector<emp::Ptr<WebArtistBase>>> artists;

  bool downloaded;

  std::string ChannelColor(ChannelPack &cp) {
    return emp::ColorHSV(
      emp::Mod(cp.size() > 1 ? cp[1] : cp[0], 360.0),
      emp::Mod(cp[0],0.6)+0.4,
      1.0
    );
  }

public:

  WebInterface(const Config & cfg_)
    : cfg(cfg_)
    , button_dash("emp_button_dash")
    , systematics_dash("emp_systematics_dash")
    , dominant_viewer("dominant_viewer")
    , w(cfg_)
    , grid_viewer("grid_viewer")
    , view_selector("view_selector")
  {

    artists.push_back({emp::NewPtr<WebArtist<ChannelPack>>(
      "Channel",
      grid_viewer,
      [this](size_t i){
        return w.man->Channel(i).GetIDs();
      },
      [this](std::optional<ChannelPack> cp) {
        return cp ? ChannelColor(*cp) : "black";
      },
      cfg_,
      [this](std::optional<ChannelPack> cp1, std::optional<ChannelPack> cp2) -> std::string {
        if (!cp1 || !cp2) return "black";
        else if ((*cp1)[0] == (*cp2)[0]) return ChannelColor(*cp1);
        else if (cp1->size() > 1 && (*cp1)[1] == (*cp2)[1]) return "white";
        else return "black";
      }
    )});

    artists.push_back({emp::NewPtr<WebArtist<double>>(
      "Stockpile",
      grid_viewer,
      [this](size_t i){
        return w.IsOccupied(i) ? std::make_optional(w.man->Stockpile(i).QueryResource()) : std::nullopt;
      },
      [this](std::optional<double> amt) -> std::string {
        if (amt) {
          if (*amt > cfg.REP_THRESH()) return emp::ColorRGB(
              std::min(255.0,(*amt - cfg.REP_THRESH())*25),
              255,
              0
            );
          else if (*amt > 0) return emp::ColorHSV(
            240,
            (*amt/ cfg.REP_THRESH()),
            1.0
          );
          else if (*amt == 0) return "white";
          else return emp::ColorRGB(
              255,
              255-std::min(255.0,(*amt)*255.0/cfg.KILL_THRESH()),
              255-std::min(255.0,(*amt)*255.0/cfg.KILL_THRESH())
            );
        } else return emp::ColorRGB(0,0,0);
      },
      cfg_
    )});

    artists.push_back({emp::NewPtr<WebArtist<double>>(
      "Resource Sharing",
      grid_viewer,
      [this](size_t i){
        return w.IsOccupied(i) ? std::make_optional(w.man->Stockpile(i).QueryTotalContribute()) : std::nullopt;
      },
      [this](std::optional<double> amt) -> std::string {
        if (amt) {
          if (*amt > cfg.REP_THRESH()) return "yellow";
          else if (*amt > 0) return emp::ColorHSV(
            240.0-180.0*(*amt)/cfg.REP_THRESH(),
            1.0,
            1.0
          );
          else if (*amt == 0) return "white";
          else return "red";
        } else return "black";
      },
      cfg_
    )});

    artists.push_back({emp::NewPtr<WebArtist<size_t>>(
      "Apoptosis",
      grid_viewer,
      [this](size_t i){
        return w.IsOccupied(i) ? std::make_optional(w.man->Apoptosis(i).GetState()) : std::nullopt;
      },
      [](std::optional<size_t> state) -> std::string {
        if (state) {
          if (*state == ManagerApoptosis::unmarked) return "white";
          else if (*state == ManagerApoptosis::partial) return "blue";
          else if (*state == ManagerApoptosis::complete) return "red";
          else return "yellow";
        } else return "black";
      },
      cfg_
    )});

    artists.push_back({emp::NewPtr<WebArtist<Genome>>(
      "Taxa",
      grid_viewer,
      [this](size_t i){
        return w.IsOccupied(i) ? std::make_optional(w.GetOrg(i)) : std::nullopt;
      },
      [](std::optional<Genome> gen) -> std::string {
        if (gen) return "white";
        else return "black";
      },
      cfg_,
      [](std::optional<Genome> g1, std::optional<Genome> g2) -> std::string {
        if (!g1 || !g2) return "black";
        else if (*g1 != *g2) return "red";
        else return "white";
      }
    )});

    artists.emplace_back();
    for (size_t l = 0; l < cfg.NLEV(); ++l) {
      artists.back().push_back(emp::NewPtr<WebArtist<int>>(
        emp::to_string("Resource Wave Level ", l),
        grid_viewer,
        [this, l](size_t i){
          return w.IsOccupied(i) ? std::make_optional(w.man->Wave(i,l).GetState()) : std::nullopt;
        },
        [this, l](std::optional<int> cp) -> std::string {
          if (cp) {
            if (*cp > 0 && *cp < cfg.Lev(l).EVENT_RADIUS()) {
              return emp::ColorRGB(0,255,0);
            } else if (*cp > 0) {
              return emp::ColorRGB(255,0,0);
            } else if (*cp < 0) {
              return emp::ColorRGB(0,0,255);
            } else {
              return "white";
            }
          } else {
            return "black";
          }
        },
        cfg
      ));
    }

    artists.emplace_back();
    for (size_t l = 0; l < cfg.NLEV() + 1; ++l) {
      artists.back().push_back(emp::NewPtr<WebArtist<size_t>>(
        emp::to_string("Reproductive Pause Level ", l),
        grid_viewer,
        [this, l](size_t i){
          return w.IsOccupied(i)
            ? std::make_optional(w.frames[i]->GetPauseSum(l))
            : std::nullopt;
        },
        [](std::optional<size_t> state) -> std::string {
          if (state) {
            if (*state == 0) return "white";
            else if (*state == 1) return "green";
            else if (*state == 2) return "blue";
            else if (*state == 3) return "purple";
            else if (*state == 4) return "red";
            else return "yellow";
          } else return "black";
        },
        cfg
      ));
    }

    artists[0][0]->Activate();

    view_selector.SetAttr(
      "class", "btn-group-toggle"
    ).SetAttr(
      "data-toggle", "buttons"
    );
    for (auto & series : artists) {
      const std::string series_id = emp::to_string(
        emp::slugify(series[0]->GetName()),
        "-outer"
      );
      view_selector << UI::Div(
        series_id
      ).SetAttr(
        "class", "btn-group d-flex"
      ).SetAttr(
        "role", "group"
      );
      for (size_t i = 0; i < series.size(); ++i) {
        const std::string target = series[i]->GetName();
        view_selector.Div(series_id) << UI::Div(emp::slugify(target)).SetAttr(
            "class",
            emp::to_string(
              "btn w-100 btn-lg m-1 btn-secondary",
              [](){
                static bool first = true;
                const bool res = first;
                first = false;
                return res;
              }() ? " active" : ""
            )
          ) << UI::Input(
              [&, target](const std::string & state){
                if (state == "true") {
                  for (auto & series : artists) {
                    for (auto & artist : series) {
                      if (artist->GetName() == target){
                        artist->Activate();
                        artist->Redraw(w.GetUpdate());
                      }
                      else artist->Deactivate();
                    }
                  }
                }
              },
              "radio",
              i ? emp::to_string(i) : target
            ).SetAttr(
              "name", "options"
            ).SetAttr(
              "autocomplete", "off"
          );
      }
    }

    auto version_text = button_dash.AddText("version_text");
    version_text << "dishtiny " << STRINGIFY(DISHTINY_HASH_);
    version_text << " / Empirical " << STRINGIFY(EMPIRICAL_HASH_);

    auto seed_text = button_dash.AddText("seed_text");
    seed_text << emp::to_string(cfg.SEED());

    auto matchbin_text = button_dash.AddText("matchbin_text");
    emp::Random temp(1); // need this to prevent a memory leak
    matchbin_text <<
      Config::hardware_t(nullptr, nullptr, &temp).GetMatchBin().name();

    auto ud_text = button_dash.AddText("ud_text");
    ud_text << "Update: " << UI::Live([this](){ return w.GetUpdate(); });

    button_dash << "&nbsp;" << UI::Button(
      [this](){ DoFrame(); },
      "Step"
    );
    button_dash << "&nbsp;" << GetToggleButton("Animate");
    button_dash << "&nbsp;" << UI::Input(
      [](std::string in){ ; },
      "checkbox",
      "Render?",
      "render_input"
    );
    button_dash << "&nbsp;" << UI::Input(
      [this](std::string _){
        if(button_dash.Input("download_input").GetCurrValue() == "true") {
          Download();
        }
      },
      "checkbox",
      "Download snapshots?",
      "download_input"
    );

    auto sys_text = systematics_dash.AddText("sys_text");
    sys_text << " Num Unique Orgs: " << UI::Live([this](){
      return w.GetSystematics("systematics")->GetNumActive();
    });
    sys_text << " Ave Depth: " << UI::Live([this](){
      return w.GetSystematics("systematics")->GetAveDepth();
    });
    sys_text << " Num Roots: " << UI::Live([this](){
      return w.GetSystematics("systematics")->GetNumRoots();
    });
    sys_text << " MRCA Depth: " << UI::Live([this](){
      return w.GetSystematics("systematics")->GetMRCADepth();
    });

    auto dom_text = dominant_viewer.AddText("dom_text");
    dom_text << UI::Live([this](){

      std::map<Genome, size_t> counts;

      for(size_t pos = 0; pos < w.GetSize(); ++pos) {
        if (w.IsOccupied(pos)) counts[w.GetOrg(pos)] ++;
      }

      using pair_type = decltype(counts)::value_type;
      auto pr = std::max_element(
        std::begin(counts),
        std::end(counts),
        [](const pair_type & p1, const pair_type & p2) {
          return p1.second < p2.second;
        }
      );

      std::ostringstream buffer;
      pr->first.program.PrintProgram(buffer);

      return "COUNT:" + emp::to_string(pr->second) + "\n\n" + buffer.str();
    });

  }

  ~WebInterface() {
    for (auto & series : artists) for (auto & ptr : series) ptr.Delete();
  }

  void DoFrame() {
    downloaded = false;

    w.Update();

    button_dash.Text("ud_text").Redraw();

    if (button_dash.Input("render_input").GetCurrValue() == "true") {
      Redraw(w.GetUpdate());
    }

    if (button_dash.Input("download_input").GetCurrValue() == "true") {
      Download();
    }
  }

  void Redraw(const size_t update) {

    systematics_dash.Text("sys_text").Redraw();
    dominant_viewer.Text("dom_text").Redraw();
    for (auto & series : artists) {
      for (auto & artist : series) artist->Redraw(update);
    }

  }


  void Download() {

    if(!downloaded && cfg.TimingFun(w.GetUpdate())) {

      const auto namify = [this](const std::string &title) {
        return emp::keyname::pack({
          {"title", title},
          {"treat", cfg.TREATMENT_DESCRIPTOR()},
          {"seed", emp::to_string(cfg.SEED())},
          {"update", emp::to_string(w.GetUpdate())},
          {"_emp_hash", STRINGIFY(EMPIRICAL_HASH_)},
          {"_source_hash", STRINGIFY(DISHTINY_HASH_)},
          {"ext", ".png"}
        });
      };

      Redraw(w.GetUpdate());
      for (auto & series : artists) {
        for (auto & artist : series) {
          artist->Download(namify(emp::slugify(artist->GetName())));
        }
      }
    }

    downloaded = true;

  }

};
