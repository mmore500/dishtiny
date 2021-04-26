#pragma once
#ifndef DISH2_UTILITY_PARE_KEYNAME_FILENAME_IMPL_HPP_INCLUDE
#define DISH2_UTILITY_PARE_KEYNAME_FILENAME_IMPL_HPP_INCLUDE

#include <cstdio>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <utility>

#include <unistd.h>

#include "../../../third-party/conduit/include/uitsl/debug/safe_compare.hpp"
#include "../../../third-party/conduit/include/uitsl/polyfill/erase_if.hpp"
#include "../../../third-party/conduit/include/uitsl/polyfill/filesystem.hpp"
#include "../../../third-party/Empirical/include/emp/base/optional.hpp"
#include "../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../third-party/Empirical/include/emp/tools/keyname_utils.hpp"
#include "../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "sha256_reduce.hpp"
#include "strlen.hpp"
#include "to_alnum.hpp"

namespace dish2 {

namespace internal {

using kv_t = std::pair<std::string, std::string>;

constexpr const char* longlink_suffix = "@longlink";
constexpr const char* meta_suffix = ".meta";

size_t get_longlinked_filename_max( const std::filesystem::path& path ) {

  constexpr size_t reserved
    = dish2::strlen(longlink_suffix) + dish2::strlen(meta_suffix);

  emp_assert( uitsl::safe_greater(
    pathconf( path.c_str(), _PC_NAME_MAX ), reserved
  ) );

  return pathconf( path.c_str(), _PC_NAME_MAX ) - reserved;

}

std::string strip_underscore_keys( const std::string& filename ){

  auto attrs = emp::keyname::unpack( filename );
  std::erase_if( attrs, []( const auto& kv ){
    return kv.first.size() && kv.first[0] == '_';
  } );
  return emp::keyname::pack( attrs );

}

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
    "longlink@", dish2::to_alnum(dish2::sha256_reduce( longest_value ))
  );

  if ( candidate_replacement.size() < longest_value.size() ) {
    auto out_attrs = emp::keyname::unpack( filename );
    out_attrs[ key ] = candidate_replacement;
    return emp::keyname::pack( out_attrs );
  } else if ( !try_a ) return longlink_longest_value(filename, true);
  else return std::nullopt;

}

std::string longlink_entire_filename(
  const std::string& filename,
  const std::filesystem::path& path,
  const bool try_a=true
) {

  auto attrs = emp::keyname::unpack( filename );

  std::erase_if( attrs, [try_a]( const auto& kv ){
    const auto& [k, v] = kv;
    const bool keep = k == "ext" || (try_a && k == "a");
    return !keep;
  });

  const std::string uid = dish2::to_alnum( dish2::sha256_reduce(filename) );

  attrs["longlink"] = uid;

  const auto res = emp::keyname::pack( attrs );
  if ( res.size() <= internal::get_longlinked_filename_max( path ) ) return res;
  else if ( try_a ) return longlink_entire_filename( filename, path, false );
  else return emp::keyname::pack({
    {"a", uid},
    {"ext", ".longlinked"}
  });

}

} // namespace internal

std::filesystem::path make_longlinked_metadata_path( std::filesystem::path p ) {

  p += internal::longlink_suffix;

  emp_assert(
    uitsl::safe_leq(
      p.filename().string().size(),
      pathconf(std::filesystem::absolute(p).parent_path().c_str(), _PC_NAME_MAX)
    ),
    p.filename(), p.filename().string().size(),
    p, std::filesystem::absolute(p).parent_path(),
    pathconf( std::filesystem::absolute(p).parent_path().c_str(), _PC_NAME_MAX )
  );

  return p;

}


std::string pare_keyname_filename_impl(
  const std::string& original_filename, const std::filesystem::path& path="./"
) {

  emp_assert( std::filesystem::exists( path ) );

  const size_t filename_max = pathconf( path.c_str(), _PC_NAME_MAX );
  const size_t longlinked_filename_max
    = internal::get_longlinked_filename_max( path );

  const size_t projected_size
    = original_filename.size() + std::strlen(internal::meta_suffix);
  if ( projected_size <= filename_max ) return original_filename;

  std::string filename = internal::strip_underscore_keys( original_filename );


  while ( filename.size() > longlinked_filename_max ) {

    const auto res = internal::longlink_longest_value( filename );

    if ( res ) filename = *res;
    else filename = internal::longlink_entire_filename(original_filename, path);

  }

  emp_assert( filename.size() <= internal::get_longlinked_filename_max(path) );

  // create additional suffixed-file at path/ containing original filename
  const auto meta_path = dish2::make_longlinked_metadata_path(path / filename);
  std::ofstream( meta_path ) << original_filename << '\n';

  emp_assert( std::filesystem::exists( meta_path ) );

  // return shortened filename
  emp_assert( filename.size() <= filename_max );
  return filename;

}

} // namespace dish2

#endif // #ifndef DISH2_UTILITY_PARE_KEYNAME_FILENAME_IMPL_HPP_INCLUDE
