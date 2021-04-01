#pragma once
#ifndef DISH2_WEB_VIEWERPANEL_HPP_INCLUDE
#define DISH2_WEB_VIEWERPANEL_HPP_INCLUDE

#include <type_traits>
#include <utility>

#include "../../../third-party/Empirical/include/emp/base/optional.hpp"
#include "../../../third-party/Empirical/include/emp/web/DocuExtras.hpp"
#include "../../../third-party/Empirical/include/emp/web/Document.hpp"
#include "../../../third-party/Empirical/include/emp/web/js_utils.hpp"

#include "../world/ThreadWorld.hpp"

#include "DocumentHandles.hpp"
#include "ViewerCollection.hpp"

namespace dish2 {

template< typename Spec >
class ViewerPanel {

  emp::web::DocuExtras view_selector{ "view_selector" };

  dish2::ViewerCollection< Spec > viewer_collection;

public:

  // adapted from https://stackoverflow.com/a/22561209
  template <typename... TupElem>
  ViewerPanel(const dish2::ThreadWorld<Spec>& thread_world)
  : viewer_collection( thread_world ) {

    dish2::document_handles.at("grid_viewer")->SetAttr(
      "class", "mx-auto card-deck"
    ).SetCSS(
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

  }

  void Redraw() { viewer_collection.Redraw(); }



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
