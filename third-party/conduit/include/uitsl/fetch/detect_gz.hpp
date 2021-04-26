#pragma once
#ifndef UITSL_FETCH_DETECT_GZ_HPP_INCLUDE
#define UITSL_FETCH_DETECT_GZ_HPP_INCLUDE

#include <fstream>
#include <istream>

#include "../nonce/ScopeGuard.hpp"
#include "../polyfill/filesystem.hpp"

namespace uitsl {

inline bool detect_gz( const std::filesystem::path& path ) {

  char buffer[258];

  std::ifstream file;
  const uitsl::ScopeGuard guard(
    [&](){ file.open( path, std::ios::binary ); },
    [&](){ file.close(); }
  );

  file.read( buffer, 258 );

  // see https://en.wikipedia.org/wiki/List_of_file_signatures
  return (buffer[0] == 31) && (buffer[1] == -117);

}

} // namespace uitsl

#endif // #ifndef UITSL_FETCH_DETECT_GZ_HPP_INCLUDE
