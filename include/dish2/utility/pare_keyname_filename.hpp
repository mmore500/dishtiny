#pragma once
#ifndef DISH2_UTILITY_PARE_KEYNAME_FILENAME_HPP_INCLUDE
#define DISH2_UTILITY_PARE_KEYNAME_FILENAME_HPP_INCLUDE

#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <stdlib.h>

#include "../../../third-party/conduit/include/uitsl/polyfill/erase_if.hpp"
#include "../../../third-party/conduit/include/uitsl/polyfill/filesystem.hpp"
#include "../../../third-party/Empirical/include/emp/base/optional.hpp"
#include "../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../third-party/Empirical/include/emp/tools/keyname_utils.hpp"
#include "../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "to_alnum.hpp"

namespace dish2 {

namespace internal {

constexpr size_t LONGLINK_FILENAME_MAX = FILENAME_MAX - 16;

using kv_t = std::pair<std::string, std::string>;

emp::optional<std::string> longlink_longest_value(
  const std::string& filename, const bool try_a=false
) {

  // get the key longest value
  auto attrs = emp::keyname::unpack( filename );
  if ( !try_a ) attrs.erase( "a" );
  attrs.erase( "_" );
  attrs.erase( "ext" );

  // exclude existing longlinks
  std::erase_if(
    attrs, []( const auto& kv ){ return kv.second.rfind( "longlink@" ) == 0; }
  );

  const auto& [key, longest_value] = *std::max_element(
    std::begin( attrs ), std::end( attrs ),
    []( const auto& left, const auto& right ){
      return left.second.size() < right.second.size();
    }
  );

  const std::string candidate_replacement = emp::to_string(
    "longlink@", dish2::to_alnum(std::hash<std::string>{}( longest_value ))
  );

  if ( candidate_replacement.size() < longest_value.size() ) {
    auto out_attrs = emp::keyname::unpack( filename );
    out_attrs[ key ] = candidate_replacement;
    return emp::keyname::pack( out_attrs );
  } else if ( !try_a ) return longlink_longest_value(filename, true);
  else return std::nullopt;

}

std::string longlink_entire_filename(
  const std::string& filename, const bool try_a=true
) {

  auto attrs = emp::keyname::unpack( filename );

  std::erase_if( attrs, [try_a]( const auto& kv ){
    const auto& [k, v] = kv;
    const bool keep = k == "ext" || (try_a && k == "a");
    return !keep;
  });

  const std::string uid = dish2::to_alnum( std::hash<std::string>{}(filename) );

  attrs["longlink"] = uid;

  const auto res = emp::keyname::pack( attrs );
  if ( res.size() <= LONGLINK_FILENAME_MAX ) return res;
  else if ( try_a ) return longlink_entire_filename( filename, false );
  else return emp::keyname::pack({
    {"a", uid},
    {"ext", ".longlink"}
  });

}

} // namespace internal

std::string pare_keyname_filename(
  const std::string& original_filename, const std::filesystem::path& path="./"
) {

  if ( original_filename.size() <= FILENAME_MAX ) return original_filename;

  std::string filename( original_filename );

  while ( filename.size() > internal::LONGLINK_FILENAME_MAX ) {

    const auto res = internal::longlink_longest_value( filename );

    if ( res ) filename = *res;
    else filename = internal::longlink_entire_filename( original_filename );

  }

  std::cout << "paring filename " << original_filename
    << " -> " << filename << std::endl;
  std::ofstream( path / ( filename + "@longlink" ) )
    << original_filename << std::endl;

  emp_assert( filename.size() <= FILENAME_MAX );

  return filename;

}

} // namespace dish2

#endif // #ifndef DISH2_UTILITY_PARE_KEYNAME_FILENAME_HPP_INCLUDE
