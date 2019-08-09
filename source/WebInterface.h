#pragma once

#include <algorithm>
#include <optional>
#include <string>
#include <stdexcept>

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
  bool download;

  bool render;

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
    , downloaded(false)
    , download(false)
    , render(true)
  {

    grid_viewer.SetAttr("class", "mx-auto");

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
      "Resource Stockpile",
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
        auto & artist = series[i];
        const std::string target = artist->GetName();
        view_selector.Div(series_id) << UI::Div(emp::slugify(target)).SetAttr(
            "class",
            emp::to_string(
              std::string(
                "btn btn-lg m-1 btn-primary"
              ) + std::string(i ? " w-25" : " w-100"),
              [](){
                static bool first = true;
                const bool res = first;
                first = false;
                return res;
              }() ? " active" : ""
            )
          ).SetAttr(
            "onclick",
            emp::to_string(
              "emp.Callback(",
              emp::JSWrap(std::function<void()>(
                [&, artist](){
                  std::cout << "running " << std::endl;
                  for (auto & s : artists) {
                    for (auto & a : s) a->Deactivate();
                  }
                  artist->Activate();
                  artist->Redraw(w.GetUpdate());
                })),
              ");"
            )
          ) << UI::Input(
              [](const std::string & state){ ; },
              "radio",
              i ? emp::to_string(i) : target
            ).SetAttr(
              "name", "view_mode"
            ).Value(
              emp::slugify(target)
            ).SetAttr(
              "autocomplete", "off"
            ).SetAttr(
              "onclick", "console.log(\"bang\");"
          );

      }
    }

    UI::Document("version_text")
      << "dishtiny " << STRINGIFY(DISHTINY_HASH_)
      << " / Empirical " << STRINGIFY(EMPIRICAL_HASH_);

    UI::Document("seed_text") << emp::to_string(cfg.SEED());

    emp::Random temp(1); // need this to prevent a memory leak
    UI::Document("matchbin_text")
      << Config::hardware_t(nullptr, nullptr, &temp).GetMatchBin().name();

    button_dash << UI::Div(
      "button_row"
    ).SetAttr(
      "class", "row justify-content-md-center"
    );

    button_dash.Div("button_row") << UI::Div(
      "step_col"
    ).SetAttr(
      "class", "col-lg-auto p-2"
    ) << UI::Button(
      [this](){ DoFrame(); },
      emp::to_string("Update ", w.GetUpdate()),
      "step_button"
    ).SetAttr(
      "class", "btn btn-block btn-lg btn-primary"
    );

    button_dash.Div("button_row") << UI::Div(
      "run_col"
    ).SetAttr(
      "class", "col-lg-auto p-2"
    ) << GetToggleButton("Animate", "Run&nbsp;", "Stop").SetAttr(
      "class", "btn btn-block btn-lg btn-primary"
    ).SetAttr(
      "data-toggle", "button"
    );

    button_dash.Div("button_row") << UI::Div(
      "render_col"
    ).SetAttr(
      "class", "col-lg-auto p-2"
    ) << UI::Div(
      "render-wrapper"
    ).SetAttr(
      "class", "input-group input-group-lg btn-block"
    ) << UI::Div(
      "render_input-prepend"
    ).SetAttr(
      "class", "input-group-prepend"
    ) << UI::Button(
      [this](){ render = !render; },
      "Render every"
    ).SetAttr(
      "aria-pressed", "true"
    ).SetAttr(
      "class", "btn active btn-primary"
    ).SetAttr(
      "data-toggle", "button"
    ).SetAttr(
      "autocomplete", "off"
    );

    button_dash.Div("render-wrapper") << UI::Input(
      [](std::string in){ ; },
      "number",
      "",
      "render_frequency"
    ).Checker(
      [](std::string in){
        return (
          emp::is_digits(in)
          && in.size()
          && std::stoi(in) > 0
        );
      }
    ).Value(
      "100"
    ).Min(
      "1"
    ).Max(
      emp::to_string(std::numeric_limits<size_t>::max())
    ).Step(
      "1"
    ).SetAttr(
      "oninput", "validity.valid || (value='1');"
    ).SetAttr(
      "class", "form-control"
    );

    button_dash.Div("render-wrapper") << UI::Div(
      "input-group-append"
    ).SetAttr(
      "class", "input-group-append"
    ) << UI::Text().SetAttr(
       "class", "input-group-text"
    ) << "th update";

    // "oninput", "validity.valid||(value='');"
    // .SetAttr(
    //   "style", "width: 5em;"
    // );

    button_dash.Div("button_row") << UI::Div(
      "download_col"
    ).SetAttr(
      "class", "col-lg-auto p-2"
    ) << UI::Button(
      [this](){ download = !download; },
      "Download snapshots?",
      "download_input"
    ).SetAttr(
      "class", "btn btn-block btn-lg btn-primary"
    ).SetAttr(
      "data-toggle", "button"
    ).SetAttr(
      "autocomplete", "off"
    );

    // button_dash.Input("render_input").RefreshCurrValue();

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

    button_dash.Button("step_button").SetLabel(
      emp::to_string("Update ", w.GetUpdate())
    );

    if (render) {
      Redraw(w.GetUpdate());
    }

    if (download) {
      Download();
    }
  }

  void InitializeViewers(const size_t update) {
    for (auto & series : artists) {
      for (auto & artist : series) artist->Deactivate();
    }
    artists[0][0]->Activate();
    // artists[0][0]->Redraw(update);
  }

  void Redraw(const size_t update) {

    std::cout << button_dash.Input("render_frequency").GetCurrValue() << std::endl;

    if (
      update % std::stoi(button_dash.Input("render_frequency").GetCurrValue())
    ) return;

    std::cout << "continuing " << std::endl;

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
