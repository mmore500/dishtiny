#pragma once
#ifndef DISH2_WEB_VIEWERPANEL_HPP_INCLUDE
#define DISH2_WEB_VIEWERPANEL_HPP_INCLUDE

#include <type_traits>
#include <utility>

#include "../../../third-party/Empirical/source/web/Document.h"
#include "../../../third-party/Empirical/source/web/DocuExtras.h"
#include "../../../third-party/Empirical/source/web/js_utils.h"
#include "../../../third-party/conduit/include/uitsl/meta/TupleConstructEach.hpp"

#include "../world/ThreadWorld.hpp"
#include "../spec/Spec.hpp"

#include "ViewerCollection.hpp"

namespace dish2 {

class ViewerPanel {

  emp::web::Document grid_viewer{ "grid_viewer" };
  emp::web::DocuExtras view_selector{ "view_selector" };

  dish2::ViewerCollection viewer_collection;

public:

  // adapted from https://stackoverflow.com/a/22561209
  template <typename... TupElem>
  ViewerPanel(dish2::ThreadWorld<dish2::Spec>& thread_world)
  : viewer_collection( uitsl::TupleConstructEach<dish2::ViewerCollection>::Make(
    thread_world, grid_viewer
  ) )
  {

    grid_viewer.SetAttr("class", "mx-auto card-deck");
    grid_viewer.SetCSS(
      "min-height",
      emp::to_string(
        std::min(emp::GetViewPortSize() - 100, 500),
        "px"
      )
    );

    view_selector.SetAttr(
      "class", "btn-group-toggle"
    );

    view_selector.SetAttr(
      "data-toggle", "buttons"
    );

    // adapted from https://stackoverflow.com/a/16387374
    // std::apply(
    //   [](auto& ...x){ std::make_tuple(
    //     x = std::decay_t<decltype(x)>()...
    //   ); },
    //   viewer_collection
    // );


    // for (auto & [category, series] : artists) {
    //   for (auto & artist : series) artist->Deactivate();
    // }
    // std::find_if(
    //   std::begin(artists),
    //   std::end(artists),
    //   [](const auto &v){ return v.second[0]->GetName() == "Channel"; }
    // )->second[0]->Activate();
  }

  void Redraw() {

    // adapted from https://stackoverflow.com/a/45498003
    std::apply(
      [](auto& ...x){ ( ..., x.Redraw() ); },
      viewer_collection
    );

  }



  // std::unordered_map<std::string, UI::Document> view_subselectors{
  //   {"group_structure_category", UI::Document("group_structure_category")},
  //   {"demographics_category", UI::Document("demographics_category")},
  //   {"resource_collection_category",
  //     UI::Document("resource_collection_category")},
  //   {"sharing_category", UI::Document("sharing_category")},
  //   {"reproduction_category", UI::Document("reproduction_category")},
  //   {"apoptosis_category", UI::Document("apoptosis_category")},
  //   {"regulation_category", UI::Document("regulation_category")},
  //   {"messaging_category", UI::Document("messaging_category")}
  // };
  // std::multimap<
  //   std::string, // category names
  //   emp::vector<emp::Ptr<WebArtistBase>> // series of artists
  // > artists;


  // for (auto & [category, series] : artists) {
  //
  //   const std::string series_id = emp::to_string(
  //     emp::slugify(series[0]->GetName()),
  //     "-outer"
  //   );
  //
  //   auto & view_subselector = view_subselectors.at(category);
  //   view_subselector.SetAttr("class", "btn-group-toggle");
  //
  //   view_subselector << UI::Div(
  //     series_id
  //   ).SetAttr(
  //     "class", "btn-group d-flex"
  //   ).SetAttr(
  //     "role", "group"
  //   );
  //
  //   for (size_t i = 0; i < series.size(); ++i) {
  //
  //     auto & artist = series[i];
  //
  //     const std::string name = artist->GetName();
  //
  //     view_subselector.Div(series_id) << UI::Div(emp::slugify(name)).SetAttr(
  //         "class",
  //         emp::to_string(
  //           std::string(
  //             "btn btn-lg m-1 btn-primary"
  //           ) + std::string(i ? " w-25" : " w-100"),
  //           name == "Channel" ? " active" : ""
  //         )
  //       ).OnClick(
  //         [&, artist](){
  //           artist->Toggle();
  //           artist->Redraw(w.GetUpdate());
  //         }
  //       ) << UI::Input(
  //           [](const std::string & state){ ; },
  //           "checkbox",
  //           i ? emp::to_string(i) : name
  //         ).SetAttr(
  //           "name", "view_mode"
  //         ).Value(
  //           emp::slugify(name)
  //         ).SetAttr(
  //           "autocomplete", "off"
  //       );
  //
  //   }
  //
  // }


};

} // namespace dish2

#endif // #ifndef DISH2_WEB_VIEWERPANEL_HPP_INCLUDE
