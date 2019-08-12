#pragma once

#include <algorithm>
#include <optional>
#include <string>
#include <stdexcept>

#include "base/vector.h"
#include "web/Button.h"
#include "web/Input.h"
#include "web/web.h"
#include "web/commands.h"
#include "web/Animate.h"
#include "web/Text.h"
#include "tools/math.h"
#include "tools/Random.h"
#include "tools/keyname_utils.h"

#include "Config.h"
#include "DishWorld.h"
#include "Genome.h"
#include "WebArtist.h"

namespace UI = emp::web;

struct DataPill {

  const std::string & title;
  const std::function<std::string()> value;
  const std::string & description;
  const bool split;

  DataPill(
    const std::string & title_,
    const std::function<std::string()> value_,
    const std::string & description_,
    const bool split_=false
  ) : title(title_)
  , value(value_)
  , description(description_)
  , split(split_)
  { ; }


  template <typename IN_TYPE>
  IN_TYPE leftleft(IN_TYPE && in_val) {

    static size_t counter = 0;
    counter++;

    return in_val << UI::Div(
      ).SetAttr(
        "class", split ? "col-md-6 p-3" : "col-md-6 col-lg-4 col-xl-3 p-3"
      ) << UI::Div(
      ).SetAttr(
        "class", "card"
      ) << UI::Div(
        emp::to_string("datapill-header-", counter)
      ).SetAttr(
        "class", "card-header"
      ) << UI::Div(
        emp::to_string("datapill-wrapper-", counter)
      ).SetAttr(
        "data-toggle", "collapse",
        "href", emp::to_string("#datapill-collapse-", counter)
      ) << UI::Div(
        emp::to_string("datapill-button-", counter), "button"
      ).SetAttr(
        "class", "btn btn-block btn-primary p-0 border-0",
        "data-toggle", "active",
        "href", emp::to_string("#datapill-active-", counter)
      ) << UI::Div(
        emp::to_string("datapill-btngroup-", counter)
      ).SetAttr(
        "class", "btn-group w-100",
        "role", "group"
      ) << UI::Div(
        emp::to_string("datapill-active-", counter)
      ).SetAttr(
        "class", "btn w-100 btn-primary border-secondary"
      ) << title << UI::Close(
        emp::to_string("datapill-active-", counter)
      ) << UI::Div(
        emp::to_string("datapill-value-", counter)
      ).SetAttr(
        "class", "badge-light btn w-25 border-secondary"
      ) << value << UI::Close(
        emp::to_string("datapill-value-", counter)
      ) << UI::Close(
        emp::to_string("datapill-btngroup-", counter)
      ) << UI::Close(
        emp::to_string("datapill-button-", counter)
      ) << UI::Close(
        emp::to_string("datapill-wrapper-", counter)
      ) << UI::Close(
        emp::to_string("datapill-header-", counter)
      ) << UI::Div(
        emp::to_string("datapill-collapse-", counter)
      ).SetAttr(
        "class", "card-body collapse"
      ) << description;

    }

};

class WebInterface : public UI::Animate {

  const Config &cfg;

  UI::Document button_dash;  //< Div that contains the button dashboard.
  UI::Document systematics_dash;  //< Div that contains the systematics info.
  UI::Document dominant_viewer;
  UI::Document dynamic_config;
  UI::Document static_config;

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
    , dynamic_config("emp_dynamic_config")
    , static_config("emp_static_config")
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
        "Resource Wave",
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
        "Reproductive Pause",
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

    grid_viewer.SetCSS(
      "min-height",
      emp::to_string(
        std::min(GetViewPortWidth() - 100, 500),
        "px"
      )
    );

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
          ).OnClick(
            [&, artist](){
              for (auto & s : artists) {
                for (auto & a : s) a->Deactivate();
              }
              artist->Activate();
              artist->Redraw(w.GetUpdate());
            }
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
    ) << UI::Div(
      "run-button",
      "button"
    ).OnClick(
      [this](){
        ToggleActive();
        button_dash.Div("run-button").Redraw();
      }
    ) << UI::Live(
      [this](){
        if (GetActive()) {
          button_dash.Button("run-button").SetAttr(
            "class", "btn btn-primary btn-block btn-lg active",
            "aria-pressed", "true"
          );
          return "Stop";
        } else {
          button_dash.Button("run-button").SetAttr(
            "class", "btn btn-primary btn-block btn-lg",
            "aria-pressed", "false"
          );
          return "Run&nbsp";
        }
      }
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
      "16"
    ).Min(
      "1"
    ).Max(
      emp::to_string(std::numeric_limits<size_t>::max())
    ).Step(
      "1"
    ).SetAttr(
      "class", "form-control"
    ).SetCSS(
      "min-width", "96px"
    );

    button_dash.Div("render-wrapper") << UI::Div(
      "input-group-append"
    ).SetAttr(
      "class", "input-group-append"
    ) << UI::Text().SetAttr(
       "class", "input-group-text"
    ) << "th update";

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

    systematics_dash.SetAttr(
      "class", "row"
    );

    DataPill(
      "Unique Genotypes",
      UI::Live([this](){
        return w.GetSystematics("systematics")->GetNumActive();
      }),
      "How many unique genotypes are there in the population?",
      true
    ).leftleft(systematics_dash << "");

    DataPill(
      "Mean Phylogenetic Depth",
      UI::Live([this](){
        return w.GetSystematics("systematics")->GetAveDepth();
      }),
      "How many steps is the line of descent to extant cells?",
      true
    ).leftleft(systematics_dash << "");

    DataPill(
      "Mean Phylogenetic Depth",
      UI::Live([this](){
        return w.GetSystematics("systematics")->GetAveDepth();
      }),
      "How many steps is the line of descent to extant cells?",
      true
    ).leftleft(systematics_dash << "");

    DataPill(
      "Phylogenetic Roots",
      UI::Live([this](){
        return w.GetSystematics("systematics")->GetNumRoots();
      }),
      "How many unique root ancestors have extant offspring?",
      true
    ).leftleft(systematics_dash << "");

    DataPill(
      "MRCA Depth",
      UI::Live([this](){
        const int res = w.GetSystematics("systematics")->GetMRCADepth();
        return (
          res == -1
          ? "n/a"
          : emp::to_string(res)
        );
      }),
      "How updates have elapsed since the most recent common ancestor of all extant organisms?",
      true
    ).leftleft(systematics_dash << "");

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

    dynamic_config.SetAttr(
      "class", "row"
    );
    for (const auto & [name, entry] : cfg) {
      auto & capturable = entry;
      DataPill(
        name,
        [capturable](){ return capturable->GetValue(); },
        entry->GetDescription()
      ).leftleft(dynamic_config << "");
    }

    static_config.SetAttr(
      "class", "row"
    );

    static_config << UI::Div(
      ).SetAttr(
        "class", "col-md-6 col-lg-4 col-xl-3 p-3"
      ) << UI::Div(
      ).SetAttr(
        "class", "card"
      ) << UI::Div(
        "source-hash-header"
      ).SetAttr(
        "class", "card-header"
      ) << UI::Div(
        "source-hash-wrapper"
      ).SetAttr(
        "data-toggle", "collapse",
        "href", "#source-hash-collapse"
      ) << UI::Button(
        [](){;},
        "dishtiny Version"
      ).SetAttr(
        "class", "btn btn-block btn-primary",
        "type", "button",
        "data-toggle", "button",
        "autocomplete", "off"
      ) << UI::Close(
        "source-hash-wrapper"
      ) << UI::Close(
        "source-hash-header"
      ) << UI::Div(
        "source-hash-collapse"
      ).SetAttr(
        "class", "card-body collapse"
      ) << STRINGIFY(DISHTINY_HASH_);

    static_config << UI::Div(
      ).SetAttr(
        "class", "col-md-6 col-lg-4 col-xl-3 p-3"
      ) << UI::Div(
      ).SetAttr(
        "class", "card"
      ) << UI::Div(
        "empirical-hash-header"
      ).SetAttr(
        "class", "card-header"
      ) << UI::Div(
        "empirical-hash-wrapper"
      ).SetAttr(
        "data-toggle", "collapse",
        "href", "#empirical-hash-collapse"
      ) << UI::Button(
        [](){;},
        "Empirical Version"
      ).SetAttr(
        "class", "btn btn-block btn-primary",
        "type", "button",
        "data-toggle", "button",
        "autocomplete", "off"
      ) << UI::Close(
        "empirical-hash-wrapper"
      ) << UI::Close(
        "empirical-hash-header"
      ) << UI::Div(
        "empirical-hash-collapse"
      ).SetAttr(
        "class", "card-body collapse"
      ) << STRINGIFY(EMPIRICAL_HASH_);


      emp::Random temp(1); // need this to prevent a memory leak
      static_config << UI::Div(
        ).SetAttr(
          "class", "col-md-6 col-lg-4 col-xl-3 p-3"
        ) << UI::Div(
        ).SetAttr(
          "class", "card"
        ) << UI::Div(
          "matchbin-header"
        ).SetAttr(
          "class", "card-header"
        ) << UI::Div(
          "matchbin-wrapper"
        ).SetAttr(
          "data-toggle", "collapse",
          "href", "#matchbin-collapse"
        ) << UI::Button(
          [](){;},
          "MatchBin"
        ).SetAttr(
          "class", "btn btn-block btn-primary",
          "type", "button",
          "data-toggle", "button",
          "autocomplete", "off"
        ) << UI::Close(
          "matchbin-wrapper"
        ) << UI::Close(
          "matchbin-header"
        ) << UI::Div(
          "matchbin-collapse"
        ).SetAttr(
          "class", "card-body collapse"
        ) << Config::hardware_t(nullptr, nullptr, &temp).GetMatchBin().name();

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

    systematics_dash.Redraw();
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
