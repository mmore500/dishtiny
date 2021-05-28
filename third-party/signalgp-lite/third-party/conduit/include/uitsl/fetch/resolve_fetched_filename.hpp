#pragma once
#ifndef UITSL_FETCH_RESOLVE_FETCHED_FILENAME_HPP_INCLUDE
#define UITSL_FETCH_RESOLVE_FETCHED_FILENAME_HPP_INCLUDE

#include <fstream>
#include <regex>
#include <streambuf>
#include <string>

#include "../../../third-party/Empirical/include/emp/base/optional.hpp"
#include "../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "../debug/compare_files.hpp"
#include "../polyfill/filesystem.hpp"

#include "make_temp_dirpath.hpp"

namespace uitsl {

inline emp::optional<std::string> try_filename_from_header(
  const std::filesystem::path& headerpath
) {

  // dump header file to string
  std::ifstream headerstream(headerpath);
  const std::string header(
    (std::istreambuf_iterator<char>( headerstream )),
    std::istreambuf_iterator<char>()
  );

  std::smatch match;
  // scrape filename out of header
  if (
    std::regex_match(
      header, match, std::regex(
        ".*content-disposition: .*filename=\"(.+)\".*",
        std::regex::extended | std::regex_constants::icase
      )
    ) && match.size() == 2
  ) {
    // zeroth match is whole string, next is the first paren expression
    return match[1].str();
  } else return std::nullopt;

}

inline emp::optional<std::string> try_filename_from_url( const std::string& url ) {
  if (
    const std::filesystem::path as_path( url );
    as_path.has_extension()
  ) return emp::url_decode<false>(as_path.filename());
  else return std::nullopt;
}


/*
 * Attempt to give fetched file its proper filename
 * (inside of temporary directory). Return new (or old) path to file.
 */
inline std::filesystem::path resolve_fetched_filename(
  const std::string& url,
  const std::filesystem::path& bodypath,
  const std::filesystem::path& headerpath
) {

  const auto from_header = uitsl::try_filename_from_header( headerpath );
  const auto from_url = uitsl::try_filename_from_url( url );

  const auto resolved_filename = from_header ?: from_url;

  if ( resolved_filename.has_value() ) {
    const auto res = uitsl::make_temp_dirpath() / *resolved_filename;
    std::filesystem::rename( bodypath, res );
    return res;
  } else return bodypath;

}

} // namespace uitsl




#endif // #ifndef UITSL_FETCH_RESOLVE_FETCHED_FILENAME_HPP_INCLUDE
