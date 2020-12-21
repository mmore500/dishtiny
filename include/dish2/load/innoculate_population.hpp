#pragma once
#ifndef DISH2_LOAD_INNOCULATE_POPULATION_HPP_INCLUDE
#define DISH2_LOAD_INNOCULATE_POPULATION_HPP_INCLUDE

#include <algorithm>
#include <set>

#include "../../../third-party/Empirical/include/emp/base/always_assert.hpp"
#include "../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../third-party/Empirical/include/emp/tools/keyname_utils.hpp"
#include "../../../third-party/Empirical/include/emp/tools/string_utils.hpp"
#include "../../../third-party/conduit/include/uitsl/polyfill/filesystem.hpp"

#include "../algorithm/seed_genomes_into.hpp"
#include "../genome/Genome.hpp"
#include "../world/ThreadWorld.hpp"

#include "load_innoculum.hpp"

namespace dish2 {

template< typename Spec >
void innoculate_population(
  const size_t thread_idx, dish2::ThreadWorld<Spec>& world
) {

  emp::vector<std::filesystem::path> innoculum_paths;
  std::copy_if(
    std::filesystem::directory_iterator( "." ),
    std::filesystem::directory_iterator(),
    std::back_inserter( innoculum_paths ),
    [](const auto& entry){
      const auto attrs = emp::keyname::unpack(entry.path());
      return attrs.count("a") && attrs.at("a") == "genome"
        && attrs.count("root_id")
        && attrs.count("ext") && attrs.at("ext") == ".json";
    }
  );

  emp::vector<dish2::Genome<Spec>> innoculums;
  std::transform(
    std::begin( innoculum_paths ),
    std::end( innoculum_paths ),
    std::back_inserter( innoculums ),
    [thread_idx](const auto& path){
      return dish2::load_innoculum<Spec>(thread_idx, path);
    }
  );

  emp_always_assert( innoculums.size() );

  emp_always_assert([&](){
    std::set<size_t> root_ids;
    std::transform(
      std::begin( innoculums ), std::end( innoculums ),
      std::inserter(root_ids, std::begin(root_ids)),
      []( const auto& innoculum ){ return innoculum.root_id.GetID(); }
    );
    return root_ids.size() == innoculums.size();
  }(), emp::to_string( innoculum_paths ));

  dish2::seed_genomes_into<Spec>( innoculums, world );

  std::cout << "thread " << thread_idx
    << " loaded " << innoculums.size() << " innoculums " << std::endl;

}

} // namespace dish2

#endif // #ifndef DISH2_LOAD_INNOCULATE_POPULATION_HPP_INCLUDE
