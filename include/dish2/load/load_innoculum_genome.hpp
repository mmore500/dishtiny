#pragma once
#ifndef DISH2_LOAD_LOAD_INNOCULUM_GENOME_HPP_INCLUDE
#define DISH2_LOAD_LOAD_INNOCULUM_GENOME_HPP_INCLUDE

#include <algorithm>
#include <fstream>

#include "../../../third-party/cereal/include/cereal/archives/binary.hpp"
#include "../../../third-party/cereal/include/cereal/archives/json.hpp"
#include "../../../third-party/conduit/include/uitsl/math/math_utils.hpp"
#include "../../../third-party/conduit/include/uitsl/mpi/comm_utils.hpp"
#include "../../../third-party/conduit/include/uitsl/polyfill/filesystem.hpp"
#include "../../../third-party/Empirical/include/emp/base/always_assert.hpp"
#include "../../../third-party/Empirical/include/emp/tools/keyname_utils.hpp"
#include "../../../third-party/header-only-gzstream/include/hogzstr/gzstream.hpp"

#include "../genome/Genome.hpp"
#include "../utility/autoload.hpp"

namespace dish2 {

template< typename Spec >
dish2::Genome<Spec> load_innoculum_genome(
  const size_t thread_idx, const std::filesystem::path& path
) {

  const auto attrs = emp::keyname::unpack( path );

  // all innoculums must specify root id
  emp_always_assert(attrs.count("root_id"), path);

  auto innoculum = dish2::autoload<dish2::Genome<Spec>>( path );

  const size_t root_id = uitsl::stoszt( attrs.at("root_id") );
  innoculum.root_id.SetID( root_id );

  if ( attrs.count("mutate_on_load") ) {
    const size_t num_muts = uitsl::stoszt( attrs.at("mutate_on_load") );
    std::cout  << "proc " << uitsl::get_proc_id() << " thread " << thread_idx
      << " applying " << num_muts << " mutations "
      << "to genome " << root_id << " from " << path << std::endl;
    for (size_t i{}; i < num_muts; ++i) innoculum.DoMutation();
  }

  std::cout  << "proc " << uitsl::get_proc_id() << " thread " << thread_idx
    << " loaded innoculum genome " << root_id << " from " << path << std::endl;

  return innoculum;

}

} // namespace dish2

#endif // #ifndef DISH2_LOAD_LOAD_INNOCULUM_GENOME_HPP_INCLUDE
