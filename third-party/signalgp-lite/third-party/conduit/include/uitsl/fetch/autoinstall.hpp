#pragma once
#ifndef UITSL_FETCH_AUTOINSTALL_HPP_INCLUDE
#define UITSL_FETCH_AUTOINSTALL_HPP_INCLUDE

#include <cstdio>
#include <cstdlib>
#include <string>

#include "../polyfill/filesystem.hpp"

#include "detect_gz.hpp"
#include "detect_tar.hpp"
#include "fetch.hpp"
#include "inflate.hpp"
#include "make_temp_filepath.hpp"
#include "untar.hpp"

namespace uitsl {

/*
 * Synchronously fetch data from a url to pwd.
 * If data is gzipped, unzip it.
 * Then, if data is tarred, untar it.
 * @return filename of installed or tar file unzipped.
 */
inline std::filesystem::path autoinstall( const std::string& url ) {

  const std::filesystem::path temp = uitsl::inflate_if_gzip(
    uitsl::fetch( url )
  );

  if ( uitsl::detect_tar( temp ) ) uitsl::untar( temp );
  // we can't rename() because renaming across filesystems fails
  else std::filesystem::copy(
    temp,
    std::filesystem::current_path() / temp.filename()
  );

  return temp.filename();

}

} // namespace uitsl

#endif // #ifndef UITSL_FETCH_AUTOINSTALL_HPP_INCLUDE
