#pragma once
#ifndef UITSL_FETCH_INFLATE_HPP_INCLUDE
#define UITSL_FETCH_INFLATE_HPP_INCLUDE

#include <cstdio>
#include <string>

#include "zlib.h"

#include "../debug/err_audit.hpp"
#include "../nonce/ScopeGuard.hpp"
#include "../polyfill/filesystem.hpp"

#include "detect_gz.hpp"
#include "make_temp_filepath.hpp"

#define UITSL_INFLATE_CHUNK 16384

namespace uitsl {

/*
 * Decompress from file source to file dest until stream ends or EOF.
 *
 * Adapted from
 * - https://zlib.net/zlib_how.html
 * - https://windrealm.org/tutorials/decompress-gzip-stream.php
 * - https://stackoverflow.com/questions/10195343/copy-a-file-in-a-sane-safe-and-efficient-way
 */
inline void inflate(gzFile_s* source_handle, FILE* dest_handle) {

 unsigned char buf[UITSL_INFLATE_CHUNK];

 for (
   int size = gzread(source_handle, buf, UITSL_INFLATE_CHUNK);
   size > 0;
   size = gzread(source_handle, buf, UITSL_INFLATE_CHUNK)
 ) std::fwrite(buf, 1, UITSL_INFLATE_CHUNK, dest_handle);

}

/*
 * Inflate from source path to destination path.
 * @return destination path
 */
inline std::filesystem::path inflate(
  const std::filesystem::path& source_path,
  const std::filesystem::path& dest_path
) {

  auto source_handle{ gzopen(source_path.c_str(), "rb") };
  const uitsl::ScopeGuard source_guard{
    [](){}, [&source_handle](){ gzclose( source_handle ); }
  };

  auto dest_handle{ std::fopen( dest_path.c_str(), "wb+" ) };
  const uitsl::ScopeGuard dest_guard{
    [](){}, [&dest_handle](){ std::fclose( dest_handle ); }
  };

  inflate( source_handle, dest_handle );

  return dest_path;

}

/*
 * Inflate from source path. Choose a destination path by stripping the gz
 * extension. If the source path doesn't have gz extension or the path with
 * the gz extension stripped exists, write to a temporary file.
 * @return destination path
 */
inline std::filesystem::path inflate( const std::filesystem::path& source_path ) {

  const auto with_extension_dropped
    = std::filesystem::path(source_path).replace_extension("");

  if (
    source_path.extension() == ".gz"
    && !std::filesystem::exists( with_extension_dropped )
  ) return uitsl::inflate( source_path, with_extension_dropped );
  else return uitsl::inflate( source_path, uitsl::make_temp_filepath() );
}

inline std::filesystem::path inflate_if_gzip(const std::filesystem::path& source) {
  if ( uitsl::detect_gz( source ) ) {
    return uitsl::inflate( source );
  }
  else return source;
}

} // namespace uitsl

#endif // #ifndef UITSL_FETCH_INFLATE_HPP_INCLUDE
