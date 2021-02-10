#pragma once
#ifndef DISH2_LOAD_LOAD_GENOME_HPP_INCLUDE
#define DISH2_LOAD_LOAD_GENOME_HPP_INCLUDE

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
dish2::Genome<Spec> load_genome( const std::filesystem::path& path ) {

  dish2::Genome<Spec> genome;

  if ( emp::keyname::unpack(path).count("ext") == 0 ) {
    emp_always_assert(false, path);
  } else if ( emp::keyname::unpack( path ).at("ext") == ".json" ) {
    std::ifstream fs( path );
    cereal::JSONInputArchive iarchive( fs );
    iarchive( genome );
  } else if ( emp::keyname::unpack( path ).at("ext") == ".json.gz" ) {
    hogzstr::igzstream fs( path );
    cereal::JSONInputArchive iarchive( fs );
    iarchive( genome );
  } else if ( emp::keyname::unpack( path ).at("ext") == ".bin" ) {
    std::ifstream fs( path );
    cereal::BinaryInputArchive iarchive( fs );
    iarchive( genome );
  } else if ( emp::keyname::unpack( path ).at("ext") == ".bin.gz" ) {
    hogzstr::igzstream fs( path );
    cereal::BinaryInputArchive iarchive( fs );
    iarchive( genome );
  } else {
    emp_always_assert(false, path, emp::keyname::unpack( path ).at("ext"));
  }

  return genome;

}

} // namespace dish2

#endif // #ifndef DISH2_LOAD_LOAD_GENOME_HPP_INCLUDE
