#pragma once
#ifndef UITSL_FETCH_DETECT_TAR_HPP_INCLUDE
#define UITSL_FETCH_DETECT_TAR_HPP_INCLUDE

#include <fstream>
#include <istream>

#include "../nonce/ScopeGuard.hpp"
#include "../polyfill/filesystem.hpp"

namespace uitsl {

inline bool detect_tar( const std::filesystem::path& path ) {

  char buffer[265];

  std::ifstream file;
  const uitsl::ScopeGuard guard(
    [&](){ file.open( path, std::ios::binary ); },
    [&](){ file.close(); }
  );

  file.read( buffer, 265 );

  // see https://en.wikipedia.org/wiki/List_of_file_signatures
  const bool is_posix_tar{
    (buffer[257] == 'u')
    && (buffer[258] == 's')
    && (buffer[259] == 't')
    && (buffer[260] == 'a')
    && (buffer[261] == 'r')
    && (buffer[262] == '\0')
    && (buffer[263] == '0')
    && (buffer[264] == '0')
  };

  const bool is_gnu_tar{
    (buffer[257] == 'u')
    && (buffer[258] == 's')
    && (buffer[259] == 't')
    && (buffer[260] == 'a')
    && (buffer[261] == 'r')
    && (buffer[262] == ' ')
    && (buffer[263] == ' ')
    && (buffer[264] == '\0')
  };

  return is_posix_tar || is_gnu_tar;

}

} // namespace uitsl

#endif // #ifndef UITSL_FETCH_DETECT_TAR_HPP_INCLUDE
