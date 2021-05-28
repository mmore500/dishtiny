#pragma once
#ifndef DISH2_UTILITY_AUTOLOAD_HPP_INCLUDE
#define DISH2_UTILITY_AUTOLOAD_HPP_INCLUDE

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <sstream>

#include "../../../third-party/bxzstr/include/bxzstr.hpp"
#include "../../../third-party/cereal/include/cereal/archives/binary.hpp"
#include "../../../third-party/cereal/include/cereal/archives/json.hpp"
#include "../../../third-party/cereal/include/cereal/details/helpers.hpp"
#include "../../../third-party/conduit/include/uitsl/math/math_utils.hpp"
#include "../../../third-party/conduit/include/uitsl/mpi/comm_utils.hpp"
#include "../../../third-party/conduit/include/uitsl/polyfill/filesystem.hpp"
#include "../../../third-party/Empirical/include/emp/base/always_assert.hpp"
#include "../../../third-party/Empirical/include/emp/tools/keyname_utils.hpp"
#include "../../../third-party/header-only-gzstream/include/hogzstr/gzstream.hpp"

#include "../debug/log_msg_native.hpp"
#include "../debug/log_tee.hpp"

#include "sha256_file.hpp"

namespace dish2 {

template< typename T >
T do_autoload( const std::filesystem::path& path ) {

  T res;

  const auto attrs = emp::keyname::unpack( path );

  dish2::log_msg_native(
    path, " size (bytes): ", std::filesystem::file_size( path )
  );
  dish2::log_msg_native(
    path, " digest (raw): ", dish2::sha256_file( std::ifstream(path) )
  );

  if ( attrs.count("ext") == 0 ) {
    emp_always_assert(false, path);
  } else if ( attrs.at("ext") == ".json" ) {
    std::ifstream fs( path );
    cereal::JSONInputArchive iarchive( fs );
    iarchive( res );
  } else if ( attrs.at("ext") == ".json.gz" ) {
    dish2::log_msg_native(
      path, " digest (unzipped): ", dish2::sha256_file(hogzstr::igzstream(path))
    );
    hogzstr::igzstream fs( path );
    cereal::JSONInputArchive iarchive( fs );
    iarchive( res );
  } else if ( attrs.at("ext") == ".json.xz" ) {
    dish2::log_msg_native(
      path, " digest (unzipped): ", dish2::sha256_file(bxz::ifstream(path))
    );
    bxz::ifstream fs( path );
    cereal::JSONInputArchive iarchive( fs );
    iarchive( res );
  } else if ( attrs.at("ext") == ".bin" ) {
    std::ifstream fs( path );
    cereal::BinaryInputArchive iarchive( fs );
    iarchive( res );
  } else if ( attrs.at("ext") == ".bin.gz" ) {
    dish2::log_msg_native(
      path, " digest (unzipped): ", dish2::sha256_file(hogzstr::igzstream(path))
    );
    hogzstr::igzstream fs( path );
    cereal::BinaryInputArchive iarchive( fs );
    iarchive( res );
  } else if ( attrs.at("ext") == ".bin.xz" ) {
    dish2::log_msg_native(
      path, " digest (unzipped): ", dish2::sha256_file(bxz::ifstream(path))
    );
    bxz::ifstream fs( path );
    cereal::BinaryInputArchive iarchive( fs );
    iarchive( res );
  } else {
    emp_always_assert(false, path, attrs.at("ext"));
  }

  return res;

}

// debugging info wrapper
template< typename T >
T autoload( const std::filesystem::path& path ) {

  try {
    return do_autoload<T>( path );
  } catch( cereal::Exception& e ) {
    dish2::log_msg_native(
      path, " autoload deserialization failed with error ", e.what()
    );
  } catch(...) {
    dish2::log_msg_native( path, " autoload deserialization failed" );
  }

  std::stringstream ss;
  ss << std::ifstream( path ).rdbuf();
  const auto contents = ss.str();

  dish2::log_msg_native( "dumping leading unsigned chars" );
  for ( size_t i{}; i < std::min(32ul, contents.size()); ++i ) {
    dish2::log_tee << static_cast<unsigned char>(contents[i]) << ' ';
  }
  dish2::log_tee << '\n';

  dish2::log_msg_native( "dumping leading chars" );
  for ( size_t i{}; i < std::min(32ul, contents.size()); ++i ) {
    dish2::log_tee << contents[i];
  }
  dish2::log_tee << '\n';

  dish2::log_msg_native( "aborting" );
  std::abort();

}

} // namespace dish2

#endif // #ifndef DISH2_UTILITY_AUTOLOAD_HPP_INCLUDE
