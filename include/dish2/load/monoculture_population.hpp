#pragma once
#ifndef DISH2_LOAD_MONOCULTURE_POPULATION_HPP_INCLUDE
#define DISH2_LOAD_MONOCULTURE_POPULATION_HPP_INCLUDE

#include <algorithm>
#include <cstdlib>
#include <set>

#include "../../../third-party/conduit/include/uitsl/mpi/comm_utils.hpp"
#include "../../../third-party/conduit/include/uitsl/polyfill/filesystem.hpp"
#include "../../../third-party/Empirical/include/emp/base/always_assert.hpp"
#include "../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../third-party/Empirical/include/emp/tools/keyname_utils.hpp"
#include "../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "../algorithm/seed_genomes_into.hpp"
#include "../genome/Genome.hpp"
#include "../world/ThreadWorld.hpp"

#include "load_genome.hpp"

namespace dish2 {

template< typename Spec >
void monoculture_population(
  const size_t thread_idx, dish2::ThreadWorld<Spec>& world
) {

  emp::vector<std::filesystem::path> monoculture_paths;
  std::copy_if(
    std::filesystem::directory_iterator( "." ),
    std::filesystem::directory_iterator(),
    std::back_inserter( monoculture_paths ),
    [](const auto& entry){
      const auto attrs = emp::keyname::unpack(entry.path());
      return attrs.count("a") && attrs.at("a") == "genome"
        && attrs.count("ext") && (
          attrs.at("ext") == ".json"
          || attrs.at("ext") == ".json.gz"
          || attrs.at("ext") == ".bin"
          || attrs.at("ext") == ".bin.gz"
        );
    }
  );

  emp_always_assert(
    monoculture_paths.size() == 1,
    monoculture_paths.size(), std::system( "ls *a=genome*" )
  );

  dish2::Genome<Spec> monoculture(
    dish2::load_genome<Spec>( monoculture_paths.front() )
  );

  std::cout  << "proc " << uitsl::get_proc_id() << " thread " << thread_idx
    << " loaded monoculture from " << monoculture_paths.front() << std::endl;

  dish2::seed_genomes_into<Spec>( {monoculture}, world );

  std::cout  << "proc " << uitsl::get_proc_id() << " thread " << thread_idx
    << " applied monoculture" << std::endl;

}

} // namespace dish2

#endif // #ifndef DISH2_LOAD_MONOCULTURE_POPULATION_HPP_INCLUDE
