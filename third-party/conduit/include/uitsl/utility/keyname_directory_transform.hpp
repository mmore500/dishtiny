#pragma once
#ifndef UITSL_UTILITY_KEYNAME_DIRECTORY_TRANSFORM_HPP_INCLUDE
#define UITSL_UTILITY_KEYNAME_DIRECTORY_TRANSFORM_HPP_INCLUDE

#include <functional>
#include <string>
#include <type_traits>
#include <utility>

#include "../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../third-party/Empirical/include/emp/tools/keyname_utils.hpp"

#include "../polyfill/filesystem.hpp"
#include "../polyfill/identity.hpp"

#include "keyname_directory_filter.hpp"

namespace uitsl {

template< typename T >
auto keyname_directory_transform(
  const std::string& key,
  emp::vector<std::pair<std::string, std::string>> filters={},
  const std::filesystem::path& target=".",
  const T& parser=std::identity,
  const bool use_regex=false
) {

  filters.emplace_back(key, "*");

  using parsed_t = std::decay_t<
    typename std::invoke_result< decltype(parser), std::string >::type
  >;

  const auto targets = uitsl::keyname_directory_filter(
    filters, target, use_regex
  );

  emp::vector< parsed_t > res;
  std::transform(
    std::begin( targets ), std::end( targets ), std::back_inserter( res ),
    [&]( const auto& entry ){
      return parser( emp::keyname::unpack( entry.string() ).at( key ) );
    }
  );

  return res;

}

} // namespace uitsl

#endif // #ifndef UITSL_UTILITY_KEYNAME_DIRECTORY_TRANSFORM_HPP_INCLUDE
