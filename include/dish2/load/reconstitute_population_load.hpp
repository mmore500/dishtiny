#pragma once
#ifndef DISH2_LOAD_RECONSTITUTE_POPULATION_LOAD_HPP_INCLUDE
#define DISH2_LOAD_RECONSTITUTE_POPULATION_LOAD_HPP_INCLUDE

#include <fstream>
#include <string>
#include <tuple>
#include <unordered_map>

#include "../../../third-party/bxzstr/include/bxzstr.hpp"
#include "../../../third-party/cereal/include/cereal/archives/binary.hpp"
#include "../../../third-party/conduit/include/uitsl/polyfill/filesystem.hpp"
#include "../../../third-party/conduit/include/uitsl/utility/keyname_directory_filter.hpp"
#include "../../../third-party/Empirical/include/emp/base/always_assert.hpp"
#include "../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../third-party/Empirical/include/emp/tools/keyname_utils.hpp"
#include "../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "../algorithm/seed_genomes_into.hpp"
#include "../genome/Genome.hpp"
#include "../utility/autoload.hpp"

#include "apply_population_filters.hpp"

namespace dish2 {

template< typename Spec >
auto reconstitute_population_load() {

  const std::unordered_map<std::string, std::string> filter_attrs{
    {"a", "population"},
    {"proc", emp::to_string( uitsl::get_proc_id() )},
    {"thread", emp::to_string( dish2::thread_idx )},
    {"ext", R"(\.bin|\.bin\.gz|\.bin\.xz|\.json|\.json\.gz|\.json\.xz)"}
  };

  const auto eligible_population_paths = uitsl::keyname_directory_filter(
    filter_attrs, ".", true
  );

  emp_always_assert(
    eligible_population_paths.size() == 1,
    eligible_population_paths.size(), eligible_population_paths,
    emp::keyname::pack(filter_attrs)
  );

  using population_t = emp::vector< dish2::Genome<Spec> >;
  const population_t reconstituted = dish2::apply_population_filters<Spec>(
    dish2::autoload< population_t >( eligible_population_paths.front() ),
    eligible_population_paths.front()
  );


  return std::tuple{
    eligible_population_paths.front(),
    reconstituted
  };

}

} // namespace dish2










#endif // #ifndef DISH2_LOAD_RECONSTITUTE_POPULATION_LOAD_HPP_INCLUDE
