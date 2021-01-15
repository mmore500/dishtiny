#pragma once
#ifndef DISH2_LOAD_RECONSTITUTE_POPULATION_HPP_INCLUDE
#define DISH2_LOAD_RECONSTITUTE_POPULATION_HPP_INCLUDE

#include <fstream>

#include "../../../third-party/cereal/include/cereal/archives/binary.hpp"
#include "../../../third-party/conduit/include/uitsl/mpi/comm_utils.hpp"
#include "../../../third-party/conduit/include/uitsl/polyfill/filesystem.hpp"
#include "../../../third-party/conduit/include/uitsl/utility/keyname_directory_filter.hpp"
#include "../../../third-party/Empirical/include/emp/base/always_assert.hpp"
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

    const auto eligible_population_paths = uitsl::keyname_directory_filter({
      {"a", "population"},
      {"proc", emp::to_string( uitsl::get_proc_id() )},
      {"thread", emp::to_string( thread_idx )},
      {"ext", ".bin"}
    });

    emp_always_assert(
      eligible_population_paths.size() == 1,
      eligible_population_paths.size(), eligible_population_paths
    );

    std::fstream fs( eligible_population_paths.front() );
    cereal::BinaryInputArchive iarchive( fs );
    iarchive( reconstituted );

    std::cout << "proc " << uitsl::get_proc_id() << " thread " << thread_idx
      << " reconstituted " << eligible_population_paths.front() << std::endl;

  }

  dish2::seed_genomes_into<Spec>( reconstituted, world );

}

} // namespace dish2










#endif // #ifndef DISH2_LOAD_RECONSTITUTE_POPULATION_HPP_INCLUDE
