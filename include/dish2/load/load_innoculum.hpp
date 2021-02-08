#pragma once
#ifndef DISH2_LOAD_LOAD_INNOCULUM_HPP_INCLUDE
#define DISH2_LOAD_LOAD_INNOCULUM_HPP_INCLUDE

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

namespace dish2 {

template< typename Spec >
dish2::Genome<Spec> load_innoculum(
  const size_t thread_idx, const std::filesystem::path& path
) {

  dish2::Genome<Spec> innoculum;

  if ( emp::keyname::unpack(path).count("ext") == 0 ) {
    emp_always_assert(false, path);
  } else if ( emp::keyname::unpack( path ).at("ext") == ".json" ) {
    std::ifstream fs( path );
    cereal::JSONInputArchive iarchive( fs );
    iarchive( innoculum );
  } else if ( emp::keyname::unpack( path ).at("ext") == ".json.gz" ) {
    hogzstr::igzstream fs( path );
    cereal::JSONInputArchive iarchive( fs );
    iarchive( innoculum );
  } else if ( emp::keyname::unpack( path ).at("ext") == ".bin" ) {
    std::ifstream fs( path );
    cereal::BinaryInputArchive iarchive( fs );
    iarchive( innoculum );
  } else if ( emp::keyname::unpack( path ).at("ext") == ".bin.gz" ) {
    hogzstr::igzstream fs( path );
    cereal::BinaryInputArchive iarchive( fs );
    iarchive( innoculum );
  } else {
    emp_always_assert(false, path, emp::keyname::unpack( path ).at("ext"));
  }

  // all innoculums must specify root id
  emp_always_assert(emp::keyname::unpack( path ).count("root_id"), path);

  const size_t root_id = uitsl::stoszt(
    emp::keyname::unpack( path ).at("root_id")
  );
  innoculum.root_id.SetID( root_id );

  std::cout  << "proc " << uitsl::get_proc_id() << " thread " << thread_idx
    << " loaded innoculum " << root_id << " from " << path << std::endl;

  return innoculum;

}

} // namespace dish2

#endif // #ifndef DISH2_LOAD_LOAD_INNOCULUM_HPP_INCLUDE
