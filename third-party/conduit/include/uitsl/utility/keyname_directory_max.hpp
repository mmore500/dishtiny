#pragma once
#ifndef UITSL_UTILITY_KEYNAME_DIRECTORY_MAX_HPP_INCLUDE
#define UITSL_UTILITY_KEYNAME_DIRECTORY_MAX_HPP_INCLUDE

#include <algorithm>
#include <string>

#include "../polyfill/filesystem.hpp"
#include "../polyfill/identity.hpp"

#include "keyname_directory_transform.hpp"

namespace uitsl {

template< typename T >
auto keyname_directory_max(
  const std::string& key,
  const emp::vector<std::pair<std::string, std::string>>& filters={},
  const std::filesystem::path& target=".",
  const T& parser=std::identity
) {

  const auto transformed = uitsl::keyname_directory_transform(
    key, filters, target, parser
  );

  emp_assert( transformed.size() );

  return *std::max_element(std::begin( transformed ), std::end( transformed ));

}

} // namespace uitsl

#endif // #ifndef UITSL_UTILITY_KEYNAME_DIRECTORY_MAX_HPP_INCLUDE
