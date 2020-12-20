#pragma once
#ifndef DISH2_CONFIG_MAKE_ARG_SPECS_HPP_INCLUDE
#define DISH2_CONFIG_MAKE_ARG_SPECS_HPP_INCLUDE

#include <string>

#include "../../../third-party/conduit/include/uitsl/fetch/autoinstall.hpp"
#include "../../../third-party/Empirical/include/emp/base/optional.hpp"
#include "../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../third-party/Empirical/include/emp/config/ArgManager.hpp"

#include "cfg.hpp"

namespace dish2 {

auto make_arg_specs() {

  auto specs = emp::ArgManager::make_builtin_specs(&dish2::cfg);

  specs.merge( std::unordered_map<std::string,emp::ArgSpec>{
    {"autoinstall", emp::ArgSpec(
      1, // quota
      "urls to pass to uitsl::autoinstall", // description
      {"a"}, // aliases
      [](const emp::optional<emp::vector<std::string>>& args){ if ( args ) {
        for ( const auto& asset_url : *args ) {
          const auto filename = uitsl::autoinstall( asset_url );
          std::cout << "autoinstalling " << asset_url
            << " to " << filename << std::endl;
        }
      } }, // callback
      false, // gobble_flags
      false // flatten
    )}
  } );

  return specs;

}

} // namespace dish2

#endif // #ifndef DISH2_CONFIG_MAKE_ARG_SPECS_HPP_INCLUDE
