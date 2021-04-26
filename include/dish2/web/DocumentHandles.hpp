#pragma once
#ifndef DISH2_WEB_DOCUMENTHANDLES_HPP_INCLUDE
#define DISH2_WEB_DOCUMENTHANDLES_HPP_INCLUDE

#include <functional>

#include "../../../third-party/Empirical/include/emp/base/optional.hpp"
#include "../../../third-party/Empirical/include/emp/web/Animate.hpp"

namespace dish2 {

using doc_wrap_t = emp::optional< emp::web::Document >;
struct DocumentHandles {

  std::unordered_map<std::string, doc_wrap_t> bak;

  auto& at( const std::string& q ) {
    if ( bak.count(q) == 0 ) {
      bak[q].emplace( q );
      bak[q]->Activate();
    };
    return bak[q];
  }

} document_handles;

} // namespace dish2

#endif // #ifndef DISH2_WEB_DOCUMENTHANDLES_HPP_INCLUDE
