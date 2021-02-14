#pragma once
#ifndef DISH2_LOAD_MAKE_GENOME_SLUG_HPP_INCLUDE
#define DISH2_LOAD_MAKE_GENOME_SLUG_HPP_INCLUDE

#include <algorithm>
#include <set>
#include <string>
#include <unordered_set>

namespace dish2 {

std::string make_genome_slug( const std::string& genome_filename ) {

  const auto innoculum_attrs = emp::keyname::unpack( genome_filename );

  emp::keyname::unpack_t res_attrs;
  std::copy_if(
    std::begin( innoculum_attrs ), std::end( innoculum_attrs ),
    std::inserter( res_attrs, std::begin( res_attrs ) ),
    []( const auto& kv_pair ){ return std::unordered_set<std::string>{
      "criteria", "morph", "proc", "replicate",
      "series", "stint", "thread", "variation"
    }.count( kv_pair.first ); }
  );

  std::string res_str = emp::keyname::pack( res_attrs );
  std::replace( std::begin( res_str ), std::end( res_str ), '+', '~' );
  std::replace( std::begin( res_str ), std::end( res_str ), '=', '%' );

  return res_str;

}

} // namespace dish2

#endif // #ifndef DISH2_LOAD_MAKE_GENOME_SLUG_HPP_INCLUDE
