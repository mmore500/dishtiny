#pragma once
#ifndef UITSL_FETCH_UNTAR_HPP_INCLUDE
#define UITSL_FETCH_UNTAR_HPP_INCLUDE

#include <algorithm>
#include <charconv>
#include <fstream>
#include <numeric>
#include <streambuf>
#include <string>
#include <string_view>

#include <sys/stat.h>

#include "../../../third-party/Empirical/include/emp/base/always_assert_warning.hpp"
#include "../../../third-party/Empirical/include/emp/base/errors.hpp"
#include "../../../third-party/Empirical/include/emp/base/optional.hpp"
#include "../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "../debug/err_audit.hpp"
#include "../nonce/ScopeGuard.hpp"
#include "../polyfill/filesystem.hpp"

namespace uitsl {

/*
 * Adapted from https://github.com/libarchive/libarchive/blob/master/contrib/untar.c
 *
 * Written by Tim Kientzle, March 2009.
 *
 * Released into the public domain.
 */

namespace internal {

// alias verbose namespace
namespace stdfs = std::filesystem;

/// Parse an octal number, ignoring leading and trailing nonsense.
inline unsigned int parseoct( const std::string_view view ) {
  unsigned int res;
  std::from_chars( std::begin(view), std::end(view), res, 8 );
  return res;
}

/// Returns true if this is 512 zero bytes.
inline bool is_end_of_archive( const std::string_view buff_view ) {
  return std::all_of(
    std::begin(buff_view),
    std::end(buff_view),
    [](const char val){ return val == '\0'; }
  );
}

/// Verify the tar checksum.
/// @return true on success, false on failure
inline bool verify_checksum( const std::string_view buff_view ) {

  // standard tar checksum adds unsigned bytes
  const unsigned char* uptr{ reinterpret_cast<const unsigned char*>(
    buff_view.data()
  ) };

  return std::accumulate(
    uptr,
    uptr + 148,
    0ul
  ) + std::accumulate(
    uptr + 156,
    uptr + 512,
    0ul
  ) + 0x20 * (156 - 148) == parseoct( buff_view.substr(148, 8) );

}

/// @return true on success, false on failure
inline bool try_set_perms( const stdfs::path& path, const stdfs::perms mode ) {

  std::error_code err;

  stdfs::permissions(path, mode, err);

  if ( err ) emp::NotifyWarning( emp::to_string(
    "setting permissions for ", path, " failed with error code ", err
  ) );

  return true;

}

/// Long filenames are stored as the contents of a LongLink file.
/// If the file being unpacked had a long filename, change its filename to the
/// contents of the LongLink file.
/// @return the path of the file being unpacked, adjusted if necessary
inline stdfs::path handle_longlink( const stdfs::path& path ) {

  if (
    // if this path isn't @LongLink...
    stdfs::path("@LongLink") != stdfs::path(path)
    // ... and there exists an @LongLink file
    && stdfs::exists( stdfs::path("@LongLink") )
  ) {

    // then set the pathname to the contents of @LongLink...
    std::ifstream longlink_stream( stdfs::path("@LongLink") );
    const std::string res{
      std::istreambuf_iterator<char>(longlink_stream),
      std::istreambuf_iterator<char>()
    };
    // ... and delete the @LongLink file
    stdfs::remove( stdfs::path("@LongLink") );

    return res;
  } else return path;

}

/// Create a directory, including parent directories as necessary.
/// @return true on success, false on failure
inline bool try_mkdir( const stdfs::path& target, const stdfs::perms mode ) {

  const stdfs::path path{ handle_longlink( target ) };

  std::error_code err;
  stdfs::create_directories(path, err);

  // stdfs::create_directories is failing inside Docker container
  // so use mkdir as a backup for now
  if (! stdfs::exists(path) ) uitsl_err_audit( mkdir( path.c_str(), 0755 ) );
  emp_assert( stdfs::exists(path) );

  if ( err ) {
    emp::NotifyError( emp::to_string(
      "creating directory ", path, " failed with error code ", err
    ) );
    return false; // failure
  }

  return try_set_perms( path, mode ); // success if set perms succeeds

}

/// Create a file, including parent directory as necessary.
inline FILE* create_file( const stdfs::path& target, const stdfs::perms mode ) {

  const stdfs::path path{ handle_longlink( target ) };

  if (
    const auto parent_path{ path.parent_path() };
    !parent_path.empty()
    && target == path // don't create longlinked directory litter
  ) {
    const auto res = try_mkdir( parent_path, stdfs::perms{ 0755 } );
    if ( res == false ) return nullptr;
  }

  FILE* res = std::fopen(path.c_str(), "wb+");

  if ( !try_set_perms( path, mode ) ) {
    std::fclose( res );
    return nullptr;
  } else return res;

}

/// @return true on success, false on failure
inline bool unpack_file_chunk(
  FILE* source, FILE* dest, const size_t bytes_remaining
) {

  char buff[512];

  const size_t bytes_traversed{ std::fread( buff, 1, 512, source ) };

  if ( bytes_traversed < 512 ) {
    emp::NotifyError( emp::to_string(
      "short read: expected 512 bytes, got ", bytes_traversed
    ) );
    return false;
  }

  const size_t bytes_read{ std::min( bytes_remaining, bytes_traversed ) };

  if ( std::fwrite(buff, 1, bytes_read, dest) != bytes_read ) {
    emp::NotifyError( "failed write" );
    return false;
  }

  return true;

}


/// @return true on success, false on failure
inline bool try_unpack_file( FILE* source, FILE* dest, const size_t filesize ) {

  if (dest == nullptr) return false; // failure, bad file handle

  const uitsl::ScopeGuard guard{ [](){}, [&dest](){ std::fclose( dest ); } };

  for (
    size_t remaining = filesize;
    remaining > 0;
    remaining -= std::min(512ul, remaining)
  ) {

    if ( unpack_file_chunk( source, dest, remaining ) ) continue; // keep going
    else return false; // failure in unpack_file_chunk

  }

  return true; // success

}

/// @return true on success, false on failure
inline bool try_skip(
  const std::string category,
  const stdfs::path path,
  FILE* source,
  const size_t size
) {

  emp::NotifyWarning( emp::to_string( "ignoring ", category, " ", path ) );

  if ( size % 512 != 0 ) {
    emp::NotifyError( emp::to_string( "size not multiple of 512 ", size ) );
    return false; // failure
  }

  const auto res{ std::fseek( source, size, SEEK_CUR ) };
  if ( res != 0 ) {
    emp::NotifyError( "bad seek, error code ", res );
    return false; // failure
  };

  return true; // success

}

/// @return true on success, false on failure
inline bool try_unpack_chunk( const std::string_view buff, FILE* source ) {

  const stdfs::path path{ std::string{ buff.data() } };
  const stdfs::perms perms{ parseoct( buff.substr(100, 8) ) };
  const size_t size{ parseoct( buff.substr(124, 12) ) };

  switch ( buff[156] ) {
    case '1': return try_skip("hardlink", path, source, size);
    case '2': return try_skip("symlink", path, source, size);
    case '3': return try_skip("character device", path, source, size);
    case '4': return try_skip("block device", path, source, size);
    case '5': return try_mkdir(path, perms);
    case '6': return try_skip("FIFO", path, source, size);
    default: return try_unpack_file( source, create_file(path, perms), size );
  }

}

/// Process chunk of a tar file.
/// @return true if complete, false if incomplete, nullopt on failure
inline emp::optional<bool> try_process_chunk( FILE* source ) {

  char buff[512];
  std::string_view buff_view{ buff, 512 };

  const size_t bytes_read{ std::fread(buff, 1, 512, source) };

  if (bytes_read < 512) {
    emp::NotifyError( emp::to_string(
      "short read: expected 512, got ", bytes_read
    ) );
    return std::nullopt; // failure
  }

  if ( is_end_of_archive( buff_view ) ) return true; // success

  if ( verify_checksum( buff_view ) == false ) {
    emp::NotifyError( "checksum failure" );
    return std::nullopt;
  }

  // failure
  if ( try_unpack_chunk( buff_view, source ) == false ) return std::nullopt;

  // keep going
  return false;

}

} // namespace internal

/// Extract a tar archive.
/// @return true on success, false on failure
inline bool untar(const std::string& filename) {

  FILE *source = std::fopen(filename.c_str(), "r");

  if ( source == nullptr ) {
    emp::NotifyError( "could not open ", filename );
    return false; // failure
  }

  const uitsl::ScopeGuard guard{[](){}, [&source](){ std::fclose( source ); } };

  while ( true ) {
    const auto res{ internal::try_process_chunk( source ) };

    if ( !res.has_value() ) {
      emp::NotifyError( emp::to_string( "untar failure ", filename ) );
      return false; // failure
    }
    else if ( *res ) return true; // success
    else continue; // not done yet, keep going

  };

}

} // namespace uitsl

#endif // #ifndef UITSL_FETCH_UNTAR_HPP_INCLUDE
