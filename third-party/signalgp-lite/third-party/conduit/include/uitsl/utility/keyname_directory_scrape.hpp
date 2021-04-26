#pragma once
#ifndef UITSL_UTILITY_KEYNAME_DIRECTORY_SCRAPE_HPP_INCLUDE
#define UITSL_UTILITY_KEYNAME_DIRECTORY_SCRAPE_HPP_INCLUDE

#include <string>
#include <type_traits>

#include "../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../third-party/Empirical/include/emp/tools/keyname_utils.hpp"

#include "../polyfill/filesystem.hpp"
#include "../polyfill/identity.hpp"

namespace uitsl {

emp::vector< std::filesystem::path > keyname_directory_scrape(
  const std::string& key, const std::filesystem::path& target="."
) {

  emp::vector< std::filesystem::path > res;
  std::copy_if(
    std::filesystem::directory_iterator( target ),
    std::filesystem::directory_iterator{},
    std::back_inserter( res ),
    [&]( const auto& entry ){
      return emp::keyname::unpack( entry.path().string() ).count( key );
    }
  );

  return res;

}

} // namespace uitsl

#endif // #ifndef UITSL_UTILITY_KEYNAME_DIRECTORY_SCRAPE_HPP_INCLUDE
