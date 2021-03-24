#pragma once
#ifndef DISH2_CONFIG_MAKE_ARG_SPECS_HPP_INCLUDE
#define DISH2_CONFIG_MAKE_ARG_SPECS_HPP_INCLUDE

#include <cstdlib>
#include <string>

#include "../../../third-party/conduit/include/uitsl/fetch/autoinstall.hpp"
#include "../../../third-party/Empirical/include/emp/base/optional.hpp"
#include "../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../third-party/Empirical/include/emp/config/ArgManager.hpp"

#include "../peripheral/readable_state/introspective_state/IntrospectiveState.hpp"
#include "../peripheral/readable_state/ReadableState.hpp"
#include "../peripheral/readable_state/writable_state/WritableState.hpp"


#include "cfg.hpp"

namespace dish2 {

template< typename Spec >
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
            << " to " << filename << '\n';
        }
      } }, // callback
      false, // gobble_flags
      false // flatten
    )},
    {"print_introspective_state_size_minus_one", emp::ArgSpec(
      0, // quota
       // description
      "Should we print the number of introspective states and exit?",
      {}, // aliases
      [](const auto& args){ if ( args ) {
        static_assert( dish2::IntrospectiveState<Spec>::parent_size );
        std::cout << dish2::IntrospectiveState<Spec>::parent_size - 1;
        std::cout << '\n';
        std::exit(0);
      } }, // callback
      false, // gobble_flags
      false // flatten
    )},
    {"print_extrospective_state_size_minus_one", emp::ArgSpec(
      0, // quota
       // description
      "Should we print the number of extrospective states and exit?",
      {}, // aliases
      [](const auto& args){ if ( args ) {
        // neighbor's readable states are your extrospective states
        static_assert( dish2::ReadableState<Spec>::parent_size );
        std::cout << dish2::ReadableState<Spec>::parent_size - 1;
        std::cout << '\n';
        std::exit(0);
      } }, // callback
      false, // gobble_flags
      false // flatten
    )},
    {"print_readable_state_size_minus_one", emp::ArgSpec(
      0, // quota
      "Should we print the number of readable states and exit?", // description
      {}, // aliases
      [](const auto& args){ if ( args ) {
        static_assert( dish2::ReadableState<Spec>::parent_size );
        std::cout << dish2::ReadableState<Spec>::parent_size - 1 << '\n';
        std::exit(0);
      } }, // callback
      false, // gobble_flags
      false // flatten
    )},
    {"print_writable_state_size_minus_one", emp::ArgSpec(
      0, // quota
      "Should we print the number of writable states and exit?", // description
      {}, // aliases
      [](const auto& args){ if ( args ) {
        static_assert( dish2::WritableState<Spec>::parent_size );
        std::cout << dish2::WritableState<Spec>::parent_size - 1 << '\n';
        std::exit(0);
      } }, // callback
      false, // gobble_flags
      false // flatten
    )}

  } );

  return specs;

}

} // namespace dish2

#endif // #ifndef DISH2_CONFIG_MAKE_ARG_SPECS_HPP_INCLUDE
