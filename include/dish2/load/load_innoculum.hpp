#pragma once
#ifndef DISH2_LOAD_LOAD_INNOCULUM_HPP_INCLUDE
#define DISH2_LOAD_LOAD_INNOCULUM_HPP_INCLUDE

#include <algorithm>

#include "../../../third-party/cereal/include/cereal/archives/json.hpp"
#include "../../../third-party/conduit/include/uitsl/math/math_utils.hpp"
#include "../../../third-party/Empirical/include/emp/base/always_assert.hpp"
#include "../../../third-party/Empirical/include/emp/tools/keyname_utils.hpp"
#include "../../../third-party/conduit/include/uitsl/polyfill/filesystem.hpp"

#include "../genome/Genome.hpp"

namespace dish2 {

template< typename Spec >
dish2::Genome<Spec> load_innoculum(
  const size_t thread_idx, const std::filesystem::path& path
) {

  dish2::Genome<Spec> innoculum;

  {
    std::fstream fs( path );
    cereal::JSONInputArchive iarchive( fs );
    iarchive( innoculum );
  }

  // all innoculums must specify root id
  emp_always_assert(emp::keyname::unpack( path ).count("root_id"), path);

  const size_t root_id = uitsl::stoszt(
    emp::keyname::unpack( path ).at("root_id")
  );
  innoculum.root_id.SetID( root_id );

  std::cout << "thread " << thread_idx
    << " loaded innoculum " << root_id << " from " << path << std::endl;

  return innoculum;

}

} // namespace dish2

#endif // #ifndef DISH2_LOAD_LOAD_INNOCULUM_HPP_INCLUDE
