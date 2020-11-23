#pragma once
#ifndef DISH2_WEB_DOCUMENTHANDLES_HPP_INCLUDE
#define DISH2_WEB_DOCUMENTHANDLES_HPP_INCLUDE

#include <functional>

#include "../../../third-party/Empirical/source/base/optional.h"
#include "../../../third-party/Empirical/source/web/Animate.h"

namespace dish2 {

using doc_wrap_t = emp::optional< emp::web::Document >;
struct DocumentHandles {

  std::unordered_map<std::string, doc_wrap_t> bak;

  auto& at( const std::string& q ) {
    if ( bak.count(q) == 0 ) bak[q] = doc_wrap_t(q);
    return bak[q];
  }

} document_handles;

} // namespace dish2

#endif // #ifndef DISH2_WEB_DOCUMENTHANDLES_HPP_INCLUDE
