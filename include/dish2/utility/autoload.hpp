#pragma once
#ifndef DISH2_UTILITY_AUTOLOAD_HPP_INCLUDE
#define DISH2_UTILITY_AUTOLOAD_HPP_INCLUDE

#include <algorithm>
#include <fstream>

#include "../../../third-party/bxzstr/include/bxzstr.hpp"
#include "../../../third-party/cereal/include/cereal/archives/binary.hpp"
#include "../../../third-party/cereal/include/cereal/archives/json.hpp"
#include "../../../third-party/conduit/include/uitsl/math/math_utils.hpp"
#include "../../../third-party/conduit/include/uitsl/mpi/comm_utils.hpp"
#include "../../../third-party/conduit/include/uitsl/polyfill/filesystem.hpp"
#include "../../../third-party/Empirical/include/emp/base/always_assert.hpp"
#include "../../../third-party/Empirical/include/emp/tools/keyname_utils.hpp"
#include "../../../third-party/header-only-gzstream/include/hogzstr/gzstream.hpp"

namespace dish2 {

template< typename T >
T autoload( const std::filesystem::path& path ) {

  T res;

  const auto attrs = emp::keyname::unpack( path );

  if ( attrs.count("ext") == 0 ) {
    emp_always_assert(false, path);
  } else if ( attrs.at("ext") == ".json" ) {
    std::ifstream fs( path );
    cereal::JSONInputArchive iarchive( fs );
    iarchive( res );
  } else if ( attrs.at("ext") == ".json.gz" ) {
    hogzstr::igzstream fs( path );
    cereal::JSONInputArchive iarchive( fs );
    iarchive( res );
  } else if ( attrs.at("ext") == ".json.xz" ) {
    bxz::ifstream fs( path );
    cereal::JSONInputArchive iarchive( fs );
    iarchive( res );
  } else if ( attrs.at("ext") == ".bin" ) {
    std::ifstream fs( path );
    cereal::BinaryInputArchive iarchive( fs );
    iarchive( res );
  } else if ( attrs.at("ext") == ".bin.gz" ) {
    hogzstr::igzstream fs( path );
    cereal::BinaryInputArchive iarchive( fs );
    iarchive( res );
  } else if ( attrs.at("ext") == ".bin.xz" ) {
    bxz::ifstream fs( path );
    cereal::BinaryInputArchive iarchive( fs );
    iarchive( res );
  } else {
    emp_always_assert(false, path, attrs.at("ext"));
  }

  return res;

}

} // namespace dish2

#endif // #ifndef DISH2_UTILITY_AUTOLOAD_HPP_INCLUDE
