#pragma once
#ifndef DISH2_LOAD_LOAD_INNOCULUM_HPP_INCLUDE
#define DISH2_LOAD_LOAD_INNOCULUM_HPP_INCLUDE

#include <algorithm>
#include <fstream>

#include "../../../third-party/Empirical/include/emp/base/always_assert.hpp"
#include "../../../third-party/Empirical/include/emp/base/assert.hpp"
#include "../../../third-party/Empirical/include/emp/tools/keyname_utils.hpp"

#include "../genome/Genome.hpp"

#include "load_innoculum_genome.hpp"
#include "load_innoculum_population.hpp"

namespace dish2 {

template< typename Spec >
emp::vector<dish2::Genome<Spec>> load_innoculum(
  const size_t thread_idx, const std::filesystem::path& path
) {

  emp_assert( emp::keyname::unpack( path ).count("a") );

  const auto& a = emp::keyname::unpack( path ).at("a");

  if ( a == "genome" ) return {dish2::load_innoculum_genome<Spec>(
    thread_idx, path
  )}; else if (a == "population") return dish2::load_innoculum_population<Spec>(
    thread_idx, path
  ); else {
    emp_always_assert(false, path, thread_idx, a);
    __builtin_unreachable();
  }

}

} // namespace dish2

#endif // #ifndef DISH2_LOAD_LOAD_INNOCULUM_HPP_INCLUDE
