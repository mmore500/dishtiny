#pragma once
#ifndef DISH2_WEB_DOCUMENTHANDLES_HPP_INCLUDE
#define DISH2_WEB_DOCUMENTHANDLES_HPP_INCLUDE

#include <functional>

#include "../../../third-party/Empirical/source/base/optional.h"
#include "../../../third-party/Empirical/source/web/Animate.h"

namespace dish2 {

using doc_wrap_t = emp::optional< emp::web::Document >;
std::unordered_map<std::string, doc_wrap_t> document_handles{

  {"grid_viewer", doc_wrap_t("grid_viewer")},

  {"group_structure_category",
    doc_wrap_t("group_structure_category")},
  {"demographics_category", doc_wrap_t("demographics_category")},
  {"resource_collection_category",
    doc_wrap_t("resource_collection_category")},
  {"sharing_category", doc_wrap_t("sharing_category")},
  {"reproduction_category", doc_wrap_t("reproduction_category")},
  {"apoptosis_category", doc_wrap_t("apoptosis_category")},
  {"regulation_category", doc_wrap_t("regulation_category")},
  {"messaging_category", doc_wrap_t("messaging_category")}

};

} // namespace dish2

#endif // #ifndef DISH2_WEB_DOCUMENTHANDLES_HPP_INCLUDE
