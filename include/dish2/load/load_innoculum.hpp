#pragma once
#ifndef DISH2_LOAD_LOAD_INNOCULUM_HPP_INCLUDE
#define DISH2_LOAD_LOAD_INNOCULUM_HPP_INCLUDE

#include <algorithm>
#include <fstream>

#include "../../../third-party/conduit/include/uitsl/mpi/comm_utils.hpp"
#include "../../../third-party/Empirical/include/emp/base/always_assert.hpp"
#include "../../../third-party/Empirical/include/emp/base/assert.hpp"
#include "../../../third-party/Empirical/include/emp/tools/keyname_utils.hpp"

#include "../genome/Genome.hpp"

#include "get_innoculum_filename.hpp"
#include "load_innoculum_genome.hpp"
#include "load_innoculum_population.hpp"

namespace dish2 {

template< typename Spec >
emp::vector<dish2::Genome<Spec>> load_innoculum(
  const std::filesystem::path& path
) {

  const auto attrs = emp::keyname::unpack( path );

  emp_assert( attrs.count("a") );

  const auto& a = attrs.at("a");

  if ( a == "genome" ) return { dish2::load_innoculum_genome<Spec>(path) };
  else if ( a == "population" ) {
    return dish2::load_innoculum_population<Spec>( path );
  } else {
    emp_always_assert(false, path, a, uitsl::get_proc_id(), dish2::thread_idx);
    __builtin_unreachable();
  }

}

template< typename Spec >
emp::vector<dish2::Genome<Spec>> load_innoculum( const size_t root_id ) {

  return dish2::load_innoculum<Spec>( dish2::get_innoculum_filename(root_id) );

}

} // namespace dish2

#endif // #ifndef DISH2_LOAD_LOAD_INNOCULUM_HPP_INCLUDE
