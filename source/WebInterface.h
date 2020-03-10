#pragma once

#include <algorithm>
#include <optional>
#include <string>
#include <stdexcept>

#include "base/vector.h"
#include "web/Div.h"
#include "web/Element.h"
#include "web/Button.h"
#include "web/Input.h"
#include "web/web.h"
#include "web/commands.h"
#include "web/Animate.h"
#include "web/Text.h"
#include "tools/math.h"
#include "tools/Random.h"
#include "tools/keyname_utils.h"
#include "tools/hash_utils.h"

#include "Config.h"
#include "DishWorld.h"
#include "Genome.h"
#include "WebArtistBase.h"
#include "WebArtistCell.h"
#include "WebArtistPointer.h"
#include "WebArtistConnection.h"

namespace UI = emp::web;

struct DataPill {

  const std::string & title;
  const std::function<std::string()> value;
  const std::string & description;
  UI::Document & parent;
  const bool split;

  DataPill(
    const std::string & title_,
    const std::function<std::string()> value_,
    const std::string & description_,
    UI::Document & parent_,
    const bool split_=false
  ) : title(title_)
  , value(value_)
  , description(description_)
  , parent(parent_)
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
        emp::to_string("datapill-wrapper2-", counter)
      ).OnClick(
        [this](){
          static bool active = false;
          active = !active;
          if (active) {
            parent.Div("datapill-active").SetCSS(
              "class", "btn w-100 btn-primary border-secondary active"
            ).SetAttr(
              "aria-pressed", "true"
            );
          } else {
            parent.Div("datapill-active").SetCSS(
              "class", "btn w-100 btn-primary border-secondary"
            ).SetAttr(
              "aria-pressed", "false"
            );
          }
        }
      ) << UI::Element(
         "button", emp::to_string("datapill-button-", counter)
      ).SetAttr(
        "class", "btn btn-block btn-primary p-0 border-0",
        "data-toggle", "button"
      ) << UI::Div(
        emp::to_string("datapill-btngroup-", counter)
      ).SetAttr(
        "class", "btn-group w-100",
        "role", "group"
      ) << UI::Div(
        emp::to_string("datapill-active-", counter)
      ).SetAttr(
        "class", "btn w-100 btn-primary border-secondary"
      ).SetCSS(
        "max-width", "75%"
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
        emp::to_string("datapill-wrapper2-", counter)
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

    struct double_datum {
      double val;
      ChannelPack cp;
      operator double() const { return val; }
    };

    struct size_t_datum {
      size_t val;
      ChannelPack cp;
      operator size_t() const { return val; }
    };

    struct int_datum {
      int val;
      ChannelPack cp;
      operator size_t() const { return val; }
    };

    artists.emplace_back();
    artists.back().push_back(emp::NewPtr<WebArtistPointer<std::string>>(
      "Tester",
      "Tester",
      grid_viewer,
      [this](const size_t i, const size_t j){
        return emp::vector<std::optional<std::string>>{
          std::optional<std::string>{"blue"},
          std::optional<std::string>{"green"},
          std::optional<std::string>{"red"},
          std::optional<std::string>{"yellow"}
        }[j];
      },
      [this](std::optional<std::string> val) {
        return *val;
      },
      cfg_
    ));

    artists.emplace_back();
    size_t dir = Cardi::Dir::NumDirs;
    artists.back().push_back(emp::NewPtr<WebArtistPointer<double_datum>>(
      "Sharing Fraction", // name
      "Sharing Fraction", // description
      grid_viewer, // viewer
      [this, dir](const size_t i, const size_t d) {
        if (w.IsOccupied(i)) return std::make_optional(double_datum{
          w.man->Sharing(i).ViewSharingFrac(dir),
          *w.man->Channel(i).GetIDs()
        });
        else return std::make_optional(double_datum{0.0});
      }, // getter
      [this](const auto amt) -> std::string {
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
      }, // renderer
      cfg_,
      [](const auto & datum1, const auto & datum2) -> std::string {
        if (!datum1|| !datum2) return "black";
        else if ((datum1->cp)[0] == (datum2->cp)[0]) return "transparent";
        else if (datum1->cp.size() > 1 && (datum1->cp)[1] == (datum2->cp)[1]) return "white";
        else return "black";
      } // divider
    ));

    artists.emplace_back();
    artists.back().push_back(emp::NewPtr<WebArtistCell<ChannelPack>>(
      "Channel",
      "Channel",
      grid_viewer,
      [this](const size_t i){
        return w.man->Channel(i).GetIDs();
      },
      [this](std::optional<ChannelPack> cp) {
        return cp ? ChannelColor(*cp) : "black";
      },
      cfg_,
      [](std::optional<ChannelPack> cp1, std::optional<ChannelPack> cp2) -> std::string {
        if (!cp1 || !cp2) return "black";
        else if ((*cp1)[0] == (*cp2)[0]) return "transparent";
        else if (cp1->size() > 1 && (*cp1)[1] == (*cp2)[1]) return "white";
        else return "black";
      }
    ));

    artists.emplace_back();
    artists.back().push_back(emp::NewPtr<WebArtistCell<double_datum>>(
      "Resource Stockpile",
      "Resource Stockpile",
      grid_viewer,
      [this](const size_t i) -> std::optional<double_datum> {
        if (w.IsOccupied(i)) return std::make_optional(double_datum{
            w.man->Stockpile(i).QueryResource()
            + w.man->Stockpile(i).QueryTotalContribute(),
            *w.man->Channel(i).GetIDs()
          });
        else return std::nullopt;
      },
      [this](const auto amt) -> std::string {
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
        } else return "black";
      },
      cfg_,
      [](const auto & datum1, const auto & datum2) -> std::string {
        if (!datum1|| !datum2) return "black";
        else if ((datum1->cp)[0] == (datum2->cp)[0]) return "transparent";
        else if (datum1->cp.size() > 1 && (datum1->cp)[1] == (datum2->cp)[1]) return "white";
        else return "black";
      }
    ));

    artists.emplace_back();
    artists.back().push_back(emp::NewPtr<WebArtistCell<double_datum>>(
      "Resource Sharing",
      "Resource Sharing",
      grid_viewer,
      [this](const size_t i) -> std::optional<double_datum> {
        if (w.IsOccupied(i)) return std::make_optional(double_datum{
          w.man->Stockpile(i).QueryTotalContribute(),
          *w.man->Channel(i).GetIDs()
        });
        else return std::nullopt;
      },
      [this](const auto amt) -> std::string {
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
      cfg_,
      [](const auto & datum1, const auto & datum2) -> std::string {
        if (!datum1|| !datum2) return "black";
        else if ((datum1->cp)[0] == (datum2->cp)[0]) return "transparent";
        else if (datum1->cp.size() > 1 && (datum1->cp)[1] == (datum2->cp)[1]) return "white";
        else return "black";
      }
    ));

    artists.emplace_back();
    artists.back().push_back(emp::NewPtr<WebArtistCell<size_t_datum>>(
      "Resource Flow",
      "Resource Flow",
      grid_viewer,
      [this](const size_t i) -> std::optional<size_t_datum> {
        size_t count = 0;
        for (size_t d = 0; d < Cardi::Dir::NumDirs; ++d) {
          if (w.man->Stockpile(i).QueryExternalContribute(d)) ++count;
        }
        if (w.IsOccupied(i)) return std::make_optional(size_t_datum{
          count,
          *w.man->Channel(i).GetIDs()
        });
        else return std::nullopt;
      },
      [](const auto state) -> std::string {
        if (state) {
          if (*state == 0) return "white";
          else if (*state == 1) return "green";
          else if (*state == 2) return "blue";
          else if (*state == 3) return "purple";
          else if (*state == 4) return "red";
          else return "yellow";
        } else return "black";
      },
      cfg_,
      [](const auto & datum1, const auto & datum2) -> std::string {
        if (!datum1|| !datum2) return "black";
        else if ((datum1->cp)[0] == (datum2->cp)[0]) return "transparent";
        else if (datum1->cp.size() > 1 && (datum1->cp)[1] == (datum2->cp)[1]) return "white";
        else return "black";
      }
    ));

    artists.emplace_back();
    artists.back().push_back(emp::NewPtr<WebArtistCell<size_t_datum>>(
      "Messaging",
      "Messaging",
      grid_viewer,
      [this](const size_t i) -> std::optional<size_t_datum> {
        if (w.IsOccupied(i)) return std::make_optional(size_t_datum{
          w.man->Inbox(i).GetTraffic(),
          *w.man->Channel(i).GetIDs()
        });
        else return std::nullopt;
      },
      [](const auto state) -> std::string {
        if (state) {
          if (*state == 0) return "white";
          else if (*state == 1) return "green";
          else if (*state == 2) return "blue";
          else if (*state == 3) return "purple";
          else if (*state == 4) return "red";
          else return "yellow";
        } else return "black";
      },
      cfg_,
      [](const auto & datum1, const auto & datum2) -> std::string {
        if (!datum1|| !datum2) return "black";
        else if ((datum1->cp)[0] == (datum2->cp)[0]) return "transparent";
        else if (datum1->cp.size() > 1 && (datum1->cp)[1] == (datum2->cp)[1]) return "white";
        else return "black";
      }
    ));

    artists.emplace_back();
    artists.back().push_back(emp::NewPtr<WebArtistCell<size_t_datum>>(
      "Messaging Flow",
      "Messaging Flow",
      grid_viewer,
      [this](const size_t i) -> std::optional<size_t_datum> {
        size_t count = 0;
        for (size_t d = 0; d < Cardi::Dir::NumDirs + 1; ++d) { if (w.man->Inbox(i).GetTraffic(d)) ++count;
        }
        if (w.IsOccupied(i)) return std::make_optional(size_t_datum{
          count,
          *w.man->Channel(i).GetIDs()
        });
        else return std::nullopt;
      },
      [](const auto state) -> std::string {
        if (state) {
          if (*state == 0) return "white";
          else if (*state == 1) return "green";
          else if (*state == 2) return "blue";
          else if (*state == 3) return "purple";
          else if (*state == 4) return "red";
          else if (*state == 5) return "gray";
          else return "yellow";
        } else return "black";
      },
      cfg_,
      [](const auto & datum1, const auto & datum2) -> std::string {
        if (!datum1|| !datum2) return "black";
        else if ((datum1->cp)[0] == (datum2->cp)[0]) return "transparent";
        else if (datum1->cp.size() > 1 && (datum1->cp)[1] == (datum2->cp)[1]) return "white";
        else return "black";
      }
    ));

    artists.emplace_back();
    artists.back().push_back(emp::NewPtr<WebArtistCell<size_t_datum>>(
      "Reproduction",
      "Reproduction",
      grid_viewer,
      [this](const size_t i) -> std::optional<size_t_datum> {
        if (w.IsOccupied(i)) return std::make_optional(size_t_datum{
          w.man->Priority(i).CountRequests(),
          *w.man->Channel(i).GetIDs()
        });
        else return std::nullopt;
      },
      [](const auto state) {
        if (state) {
          if (*state == 0) return "white";
          else if (*state == 1) return "green";
          else if (*state == 2) return "blue";
          else if (*state == 3) return "purple";
          else if (*state == 4) return "red";
          else return "yellow";
        } else return "black";
      },
      cfg_,
      [](const auto & datum1, const auto & datum2) -> std::string {
        if (!datum1|| !datum2) return "black";
        else if ((datum1->cp)[0] == (datum2->cp)[0]) return "transparent";
        else if (datum1->cp.size() > 1 && (datum1->cp)[1] == (datum2->cp)[1]) return "white";
        else return "black";
      }
    ));

    artists.emplace_back();
    artists.back().push_back(emp::NewPtr<WebArtistCell<int_datum>>(
      "Apoptosis",
      "Apoptosis",
      grid_viewer,
      [this](const size_t i) -> std::optional<int_datum> {
        if (w.IsOccupied(i)) return std::make_optional(int_datum{
          w.man->Apoptosis(i).GetState(),
          *w.man->Channel(i).GetIDs()
        });
        else return std::nullopt;
      },
      [](std::optional<size_t> state) -> std::string {
        if (state) {
          if (*state == ManagerApoptosis::unmarked) return "white";
          else if (*state == ManagerApoptosis::partial) return "blue";
          else if (*state == ManagerApoptosis::complete) return "red";
          else return "yellow";
        } else return "black";
      },
      cfg_,
      [](const auto & datum1, const auto & datum2) -> std::string {
        if (!datum1|| !datum2) return "black";
        else if ((datum1->cp)[0] == (datum2->cp)[0]) return "transparent";
        else if (datum1->cp.size() > 1 && (datum1->cp)[1] == (datum2->cp)[1]) return "white";
        else return "black";
      }
    ));

    artists.emplace_back();
    artists.back().push_back(emp::NewPtr<WebArtistCell<emp::Ptr<Genome>>>(
      "Taxa",
      "Taxa",
      grid_viewer,
      [this](const size_t i){
        return w.IsOccupied(i) ? std::make_optional(w.GetOrgPtr(i)) : std::nullopt;
      },
      [](std::optional<emp::Ptr<Genome>> gen) -> std::string {
        if (gen) return "white";
        else return "black";
      },
      cfg_,
      [](std::optional<emp::Ptr<Genome>> g1, std::optional<emp::Ptr<Genome>> g2) -> std::string {
        if (!g1 || !g2) return "black";
        else if (**g1 != **g2) return "red";
        else return "white";
      }
    ));

    artists.emplace_back();
    for (size_t l = 0; l < cfg.NLEV(); ++l) {
      artists.back().push_back(emp::NewPtr<WebArtistCell<int_datum>>(
        emp::to_string("Resource Wave Level ", l),
        "Resource Wave",
        grid_viewer,
        [this, l](const size_t i) -> std::optional<int_datum> {
          if (w.IsOccupied(i)) return std::make_optional(int_datum{
            w.man->Wave(0, i,l).GetState(),
            *w.man->Channel(i).GetIDs()
          });
          else return std::nullopt;
        },
        [this, l](const auto val) -> std::string {
          if (val) {
            if (*val > 0 && *val <= cfg.Lev(l).EVENT_RADIUS()) {
              return "green";
            } else if (*val > 0) {
              return "red";
            } else if (*val < 0) {
              return "blue";
            } else {
              return "white";
            }
          } else {
            return "black";
          }
        },
        cfg_,
        [](const auto & datum1, const auto & datum2) -> std::string {
          if (!datum1|| !datum2) return "black";
          else if ((datum1->cp)[0] == (datum2->cp)[0]) return "transparent";
          else if (datum1->cp.size() > 1 && (datum1->cp)[1] == (datum2->cp)[1]) return "white";
          else return "black";
        }
      ));
    }

    artists.emplace_back();
    for (size_t l = 0; l < cfg.NLEV(); ++l) {
      artists.back().push_back(emp::NewPtr<WebArtistCell<size_t_datum>>(
        emp::to_string("Channel Generation Level ", l),
        "Channel Generation",
        grid_viewer,
        [this, l](const size_t i) -> std::optional<size_t_datum> {
          if (w.IsOccupied(i)) return std::make_optional(size_t_datum{
            w.man->Channel(i).GetGeneration(l),
            *w.man->Channel(i).GetIDs()
          });
          else return std::nullopt;
        },
        [this, l](const auto amt) -> std::string {
          const double thresh = w.man->Channel(0).CalcExpLim(l);
          if (amt) {
            if (*amt <= thresh){
              return emp::ColorRGB(
                255.0 - std::min(255.0,(*amt)*255.0/thresh),
                255.0 - std::min(255.0,(*amt)*255.0/thresh),
                255.0
              );
            } else return emp::ColorRGB(
              255.0,
              255.0 - std::min(255.0,((*amt)-thresh)*255.0/4.0),
              255.0 - std::min(255.0,((*amt)-thresh)*255.0/4.0)
            );
          } else return "black";
        },
        cfg_,
        [](const auto & datum1, const auto & datum2) -> std::string {
          if (!datum1|| !datum2) return "black";
          else if ((datum1->cp)[0] == (datum2->cp)[0]) return "transparent";
          else if (datum1->cp.size() > 1 && (datum1->cp)[1] == (datum2->cp)[1]) return "white";
          else return "black";
        }
      ));
    }

    artists.emplace_back();
    for (size_t l = 0; l < cfg.NLEV(); ++l) {
      artists.back().push_back(emp::NewPtr<WebArtistCell<double_datum>>(
        emp::to_string("Expiration Level ", l),
        "Expiration",
        grid_viewer,
        [this, l](const size_t i) -> std::optional<double_datum> {
          if (w.IsOccupied(i)) return std::make_optional(double_datum{
            w.man->Channel(i).IsExpired(l),
            *w.man->Channel(i).GetIDs()
          });
          else return std::nullopt;
        },
        [this](const auto exp) -> std::string {
          if (exp) {
            if (*exp == 0)   {
              return "white";
            } else if (*exp <= cfg.EXP_GRACE_PERIOD()) {
              return "blue";
            } else {
              return "red";
            }
          } else {
            return "black";
          }
        },
        cfg_,
        [](const auto & datum1, const auto & datum2) -> std::string {
          if (!datum1|| !datum2) return "black";
          else if ((datum1->cp)[0] == (datum2->cp)[0]) return "transparent";
          else if (datum1->cp.size() > 1 && (datum1->cp)[1] == (datum2->cp)[1]) return "white";
          else return "black";
        }
      ));
    }

    artists.emplace_back();
    for (size_t l = 0; l < cfg.NLEV() + 1; ++l) {
      artists.back().push_back(emp::NewPtr<WebArtistCell<size_t_datum>>(
        emp::to_string("Reproductive Pause Level ", l),
        "Reproductive Pause",
        grid_viewer,
        [this, l](const size_t i) -> std::optional<size_t_datum> {
          if (w.IsOccupied(i)) return std::make_optional(size_t_datum{
            w.frames[i]->GetPauseSum(l),
            *w.man->Channel(i).GetIDs()
          });
          else return std::nullopt;
        },
        [](const auto state) -> std::string {
          if (state) {
            if (*state == 0) return "white";
            else if (*state == 1) return "green";
            else if (*state == 2) return "blue";
            else if (*state == 3) return "purple";
            else if (*state == 4) return "red";
            else return "yellow";
          } else return "black";
        },
        cfg_,
        [](const auto & datum1, const auto & datum2) -> std::string {
          if (!datum1|| !datum2) return "black";
          else if ((datum1->cp)[0] == (datum2->cp)[0]) return "transparent";
          else if (datum1->cp.size() > 1 && (datum1->cp)[1] == (datum2->cp)[1]) return "white";
          else return "black";
        }
      ));
    }

    artists.emplace_back();
    artists.back().push_back(emp::NewPtr<WebArtistConnection>(
      "Connection",
      "Connection",
      grid_viewer,
      [this](const size_t i){
        emp::vector<size_t> res;
        const auto & source = w.man->Connection(i).ViewDeveloped();
        std::transform(
          std::begin(source),
          std::end(source),
          std::back_inserter(res),
          [](const auto & item){ return item.first; }
        );
        return res;
      },
      cfg_,
      w
    ));

    artists.emplace_back();
    artists.back().push_back(emp::NewPtr<WebArtistConnection>(
      "Fledgling",
      "Fledgling",
      grid_viewer,
      [this](const size_t i){
        emp::vector<size_t> res;
        const auto & source = w.man->Connection(i).ViewFledgling();
        for (const auto & set : source) {
          std::transform(
            std::begin(set),
            std::end(set),
            std::back_inserter(res),
            [](const auto & item){ return item.location; }
          );
        }
        return res;
      },
      cfg_,
      w
    ));
    /*
    artists.emplace_back();
    for (size_t d = 0; d <= Cardi::Dir::NumDirs; ++d) {
      artists.back().push_back(emp::NewPtr<WebArtistCell<double_datum>>(
        emp::to_string("Sharing Fraction Direction ", d),
        "Sharing Fraction",
        grid_viewer,
        [this, d](const size_t i) -> std::optional<double_datum> {
          if (w.IsOccupied(i)) return std::make_optional(double_datum{
            w.man->Sharing(i).ViewSharingFrac(d),
            *w.man->Channel(i).GetIDs()
          });
          else return std::nullopt;
        },
        [this](const auto amt) -> std::string {
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
        cfg_,
        [](const auto & datum1, const auto & datum2) -> std::string {
          if (!datum1|| !datum2) return "black";
          else if ((datum1->cp)[0] == (datum2->cp)[0]) return "transparent";
          else if (datum1->cp.size() > 1 && (datum1->cp)[1] == (datum2->cp)[1]) return "white";
          else return "black";
        }
      ));
    }*/
    artists.emplace_back();
    artists.back().push_back(emp::NewPtr<WebArtistPointer<double_datum>>(
      "Shared Resource", // name
      "Shared Resource", // description
      grid_viewer, // viewer
      [this, dir](const size_t i, const size_t d) -> std::optional<double_datum> {
        if (w.IsOccupied(i)) return std::make_optional(double_datum{
          w.man->Stockpile(i).QueryExternalContribute(dir),
          *w.man->Channel(i).GetIDs()
        });
        else return std::make_optional(double_datum{0.0});
      }, // getter
      [this](const auto amt) -> std::string {
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
      }, // renderer
      cfg_,
      [](const auto & datum1, const auto & datum2) -> std::string {
        if (!datum1|| !datum2) return "black";
        else if ((datum1->cp)[0] == (datum2->cp)[0]) return "transparent";
        else if (datum1->cp.size() > 1 && (datum1->cp)[1] == (datum2->cp)[1]) return "white";
        else return "black";
      } // divider
    ));
    /*
    artists.emplace_back();
    for (size_t d = 0; d <= Cardi::Dir::NumDirs; ++d) {
      artists.back().push_back(emp::NewPtr<WebArtistCell<double_datum>>(
        emp::to_string("Shared Resource Direction ", d),
        "Shared Resource",
        grid_viewer,
        [this, d](const size_t i) -> std::optional<double_datum> {
          if (w.IsOccupied(i)) return std::make_optional(double_datum{
            w.man->Stockpile(i).QueryExternalContribute(d),
            *w.man->Channel(i).GetIDs()
          });
          else return std::nullopt;
        },
        [this](const auto amt) -> std::string {
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
        cfg_,
        [](const auto & datum1, const auto & datum2) -> std::string {
          if (!datum1|| !datum2) return "black";
          else if ((datum1->cp)[0] == (datum2->cp)[0]) return "transparent";
          else if (datum1->cp.size() > 1 && (datum1->cp)[1] == (datum2->cp)[1]) return "white";
          else return "black";
        }
      ));
    }*/

    artists.emplace_back();
    for (size_t d = 0; d <= Cardi::Dir::NumDirs; ++d) {
      artists.back().push_back(emp::NewPtr<WebArtistCell<size_t_datum>>(
        emp::to_string("Messaging Direction ", d),
        "Messaging",
        grid_viewer,
        [this, d](const size_t i) -> std::optional<size_t_datum> {
          if (w.IsOccupied(i)) return std::make_optional(size_t_datum{
            w.man->Inbox(i).GetTraffic(d),
            *w.man->Channel(i).GetIDs()
          });
          else return std::nullopt;
        },
        [this](const auto amt) -> std::string {
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
        cfg_,
        [](const auto & datum1, const auto & datum2) -> std::string {
          if (!datum1|| !datum2) return "black";
          else if ((datum1->cp)[0] == (datum2->cp)[0]) return "transparent";
          else if (datum1->cp.size() > 1 && (datum1->cp)[1] == (datum2->cp)[1]) return "white";
          else return "black";
        }
      ));
    }

    artists.emplace_back();
    artists.back().push_back(emp::NewPtr<WebArtistCell<size_t>>(
      "Root",
      "Root",
      grid_viewer,
      [this](const size_t i){
        return w.IsOccupied(i)
          ? std::make_optional(w.GetOrg(i).GetRootID())
          : std::nullopt;
      },
      [](const auto root) -> std::string {
        if (!root) return "black";
        else if (*root == 1) return "red";
        else if (*root == 2) return "green";
        else if (*root == 3) return "blue";
        else {
          const size_t res = emp::hash_combine(*root, *root);
          return emp::ColorHSV(
            emp::Mod(res, 360.0),
            emp::Mod(res, 0.6)+0.4,
            1.0
          );
        }
      },
      cfg_,
      [](
        std::optional<size_t> root1,
        std::optional<size_t> root2
      ) -> std::string {
        if (!root1 || !root2) return "black";
        else if (*root1 == *root2) return "transparent";
        else return "black";
      }
    ));

    artists.emplace_back();
    artists.back().push_back(emp::NewPtr<WebArtistCell<size_t_datum>>(
      "Regulation",
      "Regulation",
      grid_viewer,
      [this](const size_t i) -> std::optional<size_t_datum> {
        size_t count = 0;
        for (size_t d = 0; d < Cardi::Dir::NumDirs; ++d) {
          const auto & regulators = w.GetFrame(i).GetFrameHardware(
            d
          ).GetHardware().GetMatchBin().GetState().regulators;
          count += std::count_if(
            std::begin(regulators),
            std::end(regulators),
            [](const auto & pair){ return pair.second.View(); }
          );
        }
        const auto & regulators = w.GetFrame(
          i
        ).GetSpiker().GetHardware().GetMatchBin().GetState().regulators;
        count += std::count_if(
          std::begin(regulators),
          std::end(regulators),
          [](const auto & pair){ return pair.second.View(); }
        );
        if (w.IsOccupied(i)) return std::make_optional(size_t_datum{
          count,
          *w.man->Channel(i).GetIDs()
        });
        else return std::nullopt;
      },
      [](const auto state) -> std::string {
        if (state) {
          if (*state == 0) return "white";
          else if (*state == 1) return "green";
          else if (*state == 2) return "blue";
          else if (*state == 3) return "purple";
          else if (*state == 4) return "red";
          else if (*state == 5) return "gray";
          else return "yellow";
        } else return "black";
      },
      cfg_,
      [](const auto & datum1, const auto & datum2) -> std::string {
        if (!datum1|| !datum2) return "black";
        else if ((datum1->cp)[0] == (datum2->cp)[0]) return "transparent";
        else if (datum1->cp.size() > 1 && (datum1->cp)[1] == (datum2->cp)[1]) return "white";
        else return "black";
      }
    ));

    artists.emplace_back();
    artists.back().push_back(emp::NewPtr<WebArtistCell<size_t_datum>>(
      "Regulation Flow",
      "Regulation Flow",
      grid_viewer,
      [this](const size_t i) -> std::optional<size_t_datum> {
        size_t count = 0;
        for (size_t d = 0; d < Cardi::Dir::NumDirs; ++d) {
          const auto & regulators = w.GetFrame(i).GetFrameHardware(
            d
          ).GetHardware().GetMatchBin().GetState().regulators;
          count += std::any_of(
            std::begin(regulators),
            std::end(regulators),
            [](const auto & pair){ return pair.second.View(); }
          );
        }
        const auto & regulators = w.GetFrame(
          i
        ).GetSpiker().GetHardware().GetMatchBin().GetState().regulators;
        count += std::any_of(
          std::begin(regulators),
          std::end(regulators),
          [](const auto & pair){ return pair.second.View(); }
        );
        if (w.IsOccupied(i)) return std::make_optional(size_t_datum{
          count,
          *w.man->Channel(i).GetIDs()
        });
        else return std::nullopt;
      },
      [](const auto state) -> std::string {
        if (state) {
          if (*state == 0) return "white";
          else if (*state == 1) return "green";
          else if (*state == 2) return "blue";
          else if (*state == 3) return "purple";
          else if (*state == 4) return "red";
          else if (*state == 5) return "gray";
          else return "yellow";
        } else return "black";
      },
      cfg_,
      [](const auto & datum1, const auto & datum2) -> std::string {
        if (!datum1|| !datum2) return "black";
        else if ((datum1->cp)[0] == (datum2->cp)[0]) return "transparent";
        else if (datum1->cp.size() > 1 && (datum1->cp)[1] == (datum2->cp)[1]) return "white";
        else return "black";
      }
    ));

    artists.emplace_back();
    for (size_t d = 0; d <= Cardi::Dir::NumDirs; ++d) {
      artists.back().push_back(emp::NewPtr<WebArtistCell<size_t_datum>>(
        emp::to_string("Regulation Direction ", d),
        "Regulation Direction",
        grid_viewer,
        [this, d](const size_t i) -> std::optional<size_t_datum> {

          const auto & regulators = (
            d < Cardi::Dir::NumDirs
            ? w.GetFrame(i).GetFrameHardware(
                d
              ).GetHardware().GetMatchBin().GetState().regulators
            : w.GetFrame(
              i
            ).GetSpiker().GetHardware().GetMatchBin().GetState().regulators
          );
          const size_t count = std::count_if(
            std::begin(regulators),
            std::end(regulators),
            [](const auto & pair){ return pair.second.View(); }
          );
          if (w.IsOccupied(i)) return std::make_optional(size_t_datum{
            count,
            *w.man->Channel(i).GetIDs()
          });
          else return std::nullopt;

        },
        [](const auto state) -> std::string {
          if (state) {
            if (*state == 0) return "white";
            else if (*state == 1) return "green";
            else if (*state == 2) return "blue";
            else if (*state == 3) return "purple";
            else if (*state == 4) return "red";
            else if (*state == 5) return "gray";
            else return "yellow";
          } else return "black";
        },
        cfg_,
        [](const auto & datum1, const auto & datum2) -> std::string {
          if (!datum1|| !datum2) return "black";
          else if ((datum1->cp)[0] == (datum2->cp)[0]) return "transparent";
          else if (datum1->cp.size() > 1 && (datum1->cp)[1] == (datum2->cp)[1]) return "white";
          else return "black";
        }
      ));
    }

    grid_viewer.SetCSS(
      "min-height",
      emp::to_string(
        std::min(GetViewPortSize() - 100, 500),
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
    ) << UI::Element(
      "button",
      "run-button"
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

    systematics_dash.SetAttr(
      "class", "row"
    );

    DataPill(
      "Unique Genotypes",
      UI::Live([this](){
        return w.GetSystematics("systematics")->GetNumActive();
      }),
      "How many unique genotypes are there in the population?",
      systematics_dash,
      true
    ).leftleft(systematics_dash << "");

    DataPill(
      "Mean Phylogenetic Depth",
      UI::Live([this](){
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(2) << w.GetSystematics("systematics")->GetAveDepth();
        return ss.str();
      }),
      "How many steps long is the line of descent to extant cells?",
      systematics_dash,
      true
    ).leftleft(systematics_dash << "");

    DataPill(
      "Phylogenetic Roots",
      UI::Live([this](){
        return w.GetSystematics("systematics")->GetNumRoots();
      }),
      "How many unique root ancestors have extant offspring?",
      systematics_dash,
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
      systematics_dash,
      true
    ).leftleft(systematics_dash << "");

    DataPill(
      "Mean Cellular Generation",
      UI::Live([this](){
        double res = 0.0;
        size_t count = 0;
        for (size_t i = 0; i < w.GetSize(); ++i) {
          if (w.IsOccupied(i)) {
            res += w.man->Family(i).GetCellGen()[0];
            ++count;
          }
        }
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(2) << res / count;
        return ss.str();
      }),
      "How many cellular generations have elapsed?",
      systematics_dash,
      true
    ).leftleft(systematics_dash << "");

    for (size_t lev = 0; lev < cfg.NLEV(); ++lev) {
      DataPill(
        emp::to_string("Mean Channel Generation Level ", lev),
        UI::Live([this, lev](){
          double res = 0.0;
          size_t count = 0;
          for (size_t i = 0; i < w.GetSize(); ++i) {
            if (w.IsOccupied(i)) {
              res += w.man->Family(i).GetCellGen()[lev+1];
              ++count;
            }
          }
          std::ostringstream ss;
          ss << std::fixed << std::setprecision(2) << res / count;
          return ss.str();
        }),
        "How many channel generations have elapsed?",
        systematics_dash,
        true
      ).leftleft(systematics_dash << "");
    }

    auto dom_text = dominant_viewer.AddText("dom_text");
    dom_text << UI::Live([this]() -> std::string {

      if (w.GetNumOrgs() == 0) return "COUNT: 0";

      const auto dominant = w.GetDominantInfo();
      std::ostringstream buffer;
      buffer << "POINTER" << std::endl;
      dominant.first.GetProgram().PrintProgram(buffer);
      buffer << std::endl << "SPIKER" << std::endl;
      dominant.first.GetProgramSpiker().PrintProgram(buffer);

      return "COUNT:" + emp::to_string(dominant.second) + "\n\n" + buffer.str();
    });

    dynamic_config.SetAttr(
      "class", "row"
    );

    // alphabetize
    const std::map<
      decltype(cfg.begin()->first),
      decltype(cfg.begin()->second)
    > alph(cfg.begin(), cfg.end());

    for (const auto & [name, entry] : alph) {
      if (
        entry->GetDescription().find("[NATIVE]") == std::string::npos
      ) {
        auto & capturable = entry;
        DataPill(
          name,
          [capturable](){ return capturable->GetValue(); },
          entry->GetDescription(),
          dynamic_config
        ).leftleft(dynamic_config << "");
      }
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
  }

  void Redraw(const size_t update) {

    if (
      update % std::stoi(button_dash.Input("render_frequency").GetCurrValue())
    ) return;

    systematics_dash.Redraw();
    dominant_viewer.Text("dom_text").Redraw();
    for (auto & series : artists) {
      for (auto & artist : series) artist->Redraw(update);
    }

  }


  void Download() {

    if (
      const auto render_frequency = std::stoi(
        button_dash.Input("render_frequency").GetCurrValue()
      );
      !downloaded
      && w.GetUpdate() % render_frequency == 0
    ) {

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
