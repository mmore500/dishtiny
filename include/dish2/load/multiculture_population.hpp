#pragma once
#ifndef DISH2_LOAD_MULTICULTURE_POPULATION_HPP_INCLUDE
#define DISH2_LOAD_MULTICULTURE_POPULATION_HPP_INCLUDE

#include <algorithm>
#include <cstdlib>
#include <vector>

#include "../../../third-party/conduit/include/uitsl/polyfill/filesystem.hpp"
#include "../../../third-party/conduit/include/uitsl/utility/keyname_directory_filter.hpp"
#include "../../../third-party/Empirical/include/emp/base/always_assert.hpp"
#include "../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../third-party/Empirical/include/emp/tools/keyname_utils.hpp"
#include "../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "../algorithm/seed_genomes_into.hpp"
#include "../debug/log_msg.hpp"
#include "../genome/Genome.hpp"
#include "../utility/autoload.hpp"
#include "../world/ThreadWorld.hpp"

namespace dish2 {

template< typename Spec >
void multiculture_population( dish2::ThreadWorld<Spec>& world ) {

  const auto multiculture_paths = uitsl::keyname_directory_filter({
    {"a", "genome"},
    {"ext", R"(\.bin|\.bin\.gz|\.bin\.xz|\.json|\.json\.gz|\.json\.xz)"}
  }, ".", true);

  emp_always_assert(
    multiculture_paths.size(),
    multiculture_paths.size(), std::system( "ls *a=genome*" )
  );

  dish2::log_msg(
    multiculture_paths.size(), " multiculture strains detected"
  );

  emp::vector<dish2::Genome<Spec>> multiculture_strains;
  multiculture_strains.reserve(multiculture_paths.size());
  std::transform(
    std::begin(multiculture_paths),
    std::end(multiculture_paths),
    std::back_inserter(multiculture_strains),
    [](const auto& multiculture_path){
      dish2::log_msg(
        " loaded multiculture strain from ", multiculture_path
      );
      return dish2::autoload<dish2::Genome<Spec>>( multiculture_path );
    }
  );

  dish2::log_msg(
    multiculture_paths.size(), " multiculture strains loaded"
  );

  dish2::seed_genomes_into<Spec, true>( multiculture_strains, world );

  dish2::log_msg( "applied multiculture" );

}

} // namespace dish2

#endif // #ifndef DISH2_LOAD_MULTICULTURE_POPULATION_HPP_INCLUDE
