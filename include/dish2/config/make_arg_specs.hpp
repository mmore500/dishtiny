#pragma once
#ifndef DISH2_CONFIG_MAKE_ARG_SPECS_HPP_INCLUDE
#define DISH2_CONFIG_MAKE_ARG_SPECS_HPP_INCLUDE

#include <algorithm>
#include <cstdlib>
#include <set>
#include <string>

#include "../../../third-party/conduit/include/uitsl/algorithm/transform_if.hpp"
#include "../../../third-party/conduit/include/uitsl/fetch/autoinstall.hpp"
#include "../../../third-party/Empirical/include/emp/base/optional.hpp"
#include "../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../third-party/Empirical/include/emp/config/ArgManager.hpp"

#include "../algorithm/get_lowest_root.hpp"
#include "../debug/log_msg.hpp"
#include "../load/reconstitute_population_load.hpp"
#include "../peripheral/readable_state/introspective_state/IntrospectiveState.hpp"
#include "../peripheral/readable_state/ReadableState.hpp"
#include "../peripheral/readable_state/writable_state/WritableState.hpp"
#include "../utility/autoload.hpp"

#include "cfg.hpp"
#include "TemporaryThreadIdxOverride.hpp"

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
          dish2::log_msg( "autoinstalling ", asset_url, " to ", filename );
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
    )},
    {"print_reconstituted_lowestroot_numstintroots", emp::ArgSpec(
      0, // quota
      "Should we reconstitute a population, " // description
      "print the number of unique stint phylogenetic roots, and exit?",
      {}, // aliases
      [](const auto& args){ if ( args ) {
        const dish2::TemporaryThreadIdxOverride guard(0);
        const auto [src, pop] = dish2::reconstitute_population_load<Spec>();
        const size_t lowest_root = dish2::get_lowest_root( pop );
        std::set<size_t> lowestroot_stintrootids;
        uitsl::transform_if(
          std::begin( pop ), std::end( pop ),
          std::inserter(
            lowestroot_stintrootids, std::begin(lowestroot_stintrootids)
          ),
          [](const auto& genome){ return genome.stint_root_id.GetID(); },
          [lowest_root](const auto& genome){
            return genome.root_id.GetID() == lowest_root;
          }
        );
        std::cout << lowestroot_stintrootids.size() << '\n';
        std::exit(0);
      } }, // callback
      false, // gobble_flags
      false // flatten
    )},
    {"print_lowestroot_numstintroots", emp::ArgSpec(
      1, // quota
      "Should we load a population, " // description
      "print the number of unique stint phylogenetic roots, and exit?",
      {}, // aliases
      [](const auto& args){ if ( args ) {
        using population_t = emp::vector< dish2::Genome<Spec> >;
        const auto pop = dish2::autoload< population_t >( args->front() );
        const size_t lowest_root = dish2::get_lowest_root( pop );
        std::set<size_t> lowestroot_stintrootids;
        uitsl::transform_if(
          std::begin( pop ), std::end( pop ),
          std::inserter(
            lowestroot_stintrootids, std::begin(lowestroot_stintrootids)
          ),
          [](const auto& genome){ return genome.stint_root_id.GetID(); },
          [lowest_root](const auto& genome){
            return genome.root_id.GetID() == lowest_root;
          }
        );
        std::cout << lowestroot_stintrootids.size() << '\n';
        std::exit(0);
      } }, // callback
      false, // gobble_flags
      false // flatten
    )},
    {"print_lowestroot", emp::ArgSpec(
      1, // quota
      "Should we load a population, print the lowest root id, and exit?",// desc
      {}, // aliases
      [](const auto& args){ if ( args ) {
        using population_t = emp::vector< dish2::Genome<Spec> >;
        const auto pop = dish2::autoload< population_t >( args->front() );
        std::cout << dish2::get_lowest_root( pop ) << '\n';
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
