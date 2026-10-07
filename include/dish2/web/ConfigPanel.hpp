#pragma once
#ifndef DISH2_WEB_CONFIGPANEL_HPP_INCLUDE
#define DISH2_WEB_CONFIGPANEL_HPP_INCLUDE

#include <map>
#include <string>

#include "../../../third-party/Empirical/include/emp/base/macros.hpp"
#include "../../../third-party/Empirical/include/emp/web/Div.hpp"
#include "../../../third-party/Empirical/include/emp/web/Document.hpp"

#include "../config/cfg.hpp"
#include "../spec/Spec.hpp"

#include "DataPill.hpp"
#include "SnippetPill.hpp"
#include "url_encode.hpp"

namespace dish2 {

class ConfigPanel {

  emp::web::Document dynamic_config{ "emp_dynamic_config" };
  emp::web::Document static_config{ "emp_static_config" };

public:

  ConfigPanel() {

    dynamic_config.SetAttr( "class", "row" );

    // alphabetize
    const std::map<
      std::string, decltype( dish2::cfg.begin()->second )
    > alph( dish2::cfg.begin(), dish2::cfg.end() );

    for ( const auto& [name, entry] : alph ) {
      // native-only parameters have no effect on the web interface
      if ( entry->GetDescription().find( "[NATIVE]" ) != std::string::npos ) {
        continue;
      }
      dynamic_config << dish2::SnippetPill(
        name,
        // structured bindings can't be captured in C++17, so copy
        [name = name, entry = entry](){
          return dish2::url_encode( name )
            + "=" + dish2::url_encode( entry->GetValue() );
        },
        entry->GetDescription()
      );
    }

    // parameters that are locked in at compile time
    static_config.SetAttr( "class", "row" );

    static_config << dish2::DataPill(
      "dishtiny Version",
      [](){ return std::string{ EMP_STRINGIFY(DISHTINY_HASH_) }; },
      "Hash of the dishtiny source code this was compiled from."
    ).pill;
    static_config << dish2::DataPill(
      "Spec",
      [](){ return std::string{ EMP_STRINGIFY(DISH2_SPEC) }; },
      "Compile-time specification this was built with."
    ).pill;

  }

};

} // namespace dish2

#endif // #ifndef DISH2_WEB_CONFIGPANEL_HPP_INCLUDE
