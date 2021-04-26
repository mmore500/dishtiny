#pragma once
#ifndef UITSL_FETCH_FETCH_NATIVE_HPP_INCLUDE
#define UITSL_FETCH_FETCH_NATIVE_HPP_INCLUDE

#include <chrono>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <thread>

#include <curl/curl.h>

#include "../../../third-party/Empirical/include/emp/base/always_assert.hpp"

#include "../debug/err_verify.hpp"
#include "../nonce/ScopeGuard.hpp"
#include "../polyfill/filesystem.hpp"

#include "make_temp_filepath.hpp"
#include "resolve_fetched_filename.hpp"

namespace uitsl {

/*
 * Fetch data from a url to a temporary file, return temporary file path.
 * Requires -lcurl.
 * Adapted from https://stackoverflow.com/a/21573625.
 */
inline std::filesystem::path fetch_native( const std::string& url ) {

  const uitsl::ScopeGuard curl_global_guard(
    [](){ curl_global_init(CURL_GLOBAL_ALL); },
    [](){ curl_global_cleanup(); }
  );

  const std::filesystem::path bodypath{ uitsl::make_temp_filepath() };
  FILE *bodyfile;
  const uitsl::ScopeGuard bodyfile_guard(
    [&bodyfile, bodypath](){ bodyfile = fopen(bodypath.c_str(), "wb"); },
    [&bodyfile](){ fclose(bodyfile); }
  );

  const std::filesystem::path headerpath{ uitsl::make_temp_filepath() };
  FILE *headerfile;
  const uitsl::ScopeGuard headerfile_guard(
    [&](){ headerfile = fopen(headerpath.c_str(), "wb"); },
    [&](){ fclose(headerfile); }
  );

  CURL* curl_handle;
  const uitsl::ScopeGuard curl_handle_guard(
    [&curl_handle](){ curl_handle = curl_easy_init(); },
    [&curl_handle](){ curl_easy_cleanup( curl_handle ); }
  );

  emp_always_assert( curl_handle );

  uitsl::err_verify(
    curl_easy_setopt( curl_handle, CURLOPT_URL, url.c_str())
  );
  uitsl::err_verify(
    curl_easy_setopt( curl_handle, CURLOPT_WRITEFUNCTION, nullptr)
  );
  uitsl::err_verify(
    curl_easy_setopt( curl_handle, CURLOPT_WRITEDATA, bodyfile)
  );
  uitsl::err_verify(
    curl_easy_setopt( curl_handle, CURLOPT_HEADERDATA, headerfile)
  );
  uitsl::err_verify(
    curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1L)
  );

  for ( size_t retry{}; true; ++retry ) {

    CURLcode err = curl_easy_perform(curl_handle);

    long http_code{};
    if ( err == CURLE_OK ) {
      curl_easy_getinfo( curl_handle, CURLINFO_RESPONSE_CODE, &http_code );
      if ( http_code == 200 ) break;
    }

    emp_always_assert( retry < 5, retry, err, http_code, url );

    using namespace std::chrono_literals;
    std::this_thread::sleep_for( 10s );
  }

  fflush( bodyfile );
  fflush( headerfile );

  return uitsl::resolve_fetched_filename( url, bodypath, headerpath );

}

} // namespace uitsl

#endif // #ifndef UITSL_FETCH_FETCH_NATIVE_HPP_INCLUDE
