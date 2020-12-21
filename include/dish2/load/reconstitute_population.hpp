#pragma once
#ifndef DISH2_LOAD_RECONSTITUTE_POPULATION_HPP_INCLUDE
#define DISH2_LOAD_RECONSTITUTE_POPULATION_HPP_INCLUDE

#include <fstream>

#include "../../../third-party/cereal/include/cereal/archives/binary.hpp"
#include "../../../third-party/conduit/include/uitsl/polyfill/filesystem.hpp"
#include "../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../third-party/Empirical/include/emp/tools/keyname_utils.hpp"
#include "../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "../algorithm/seed_genomes_into.hpp"
#include "../genome/Genome.hpp"
#include "../world/ThreadWorld.hpp"

namespace dish2 {

template< typename Spec >
void reconstitute_population(
  const size_t thread_idx, dish2::ThreadWorld<Spec>& world
) {

  // must set root ids here?
  emp::vector< dish2::Genome<Spec> > reconstituted;

  {

    const std::filesystem::path expected_population_path = emp::keyname::pack({
      {"a", "population"},
      {"thread", emp::to_string( thread_idx )},
      {"ext", ".bin"}
    });

    std::fstream fs( expected_population_path );
    cereal::BinaryInputArchive iarchive( fs );
    iarchive( reconstituted );

    std::cout << "thread " << thread_idx
      << " reconstituted " << expected_population_path << std::endl;

  }

  dish2::seed_genomes_into<Spec>( reconstituted, world );

}

} // namespace dish2










#endif // #ifndef DISH2_LOAD_RECONSTITUTE_POPULATION_HPP_INCLUDE
