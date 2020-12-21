#pragma once
#ifndef DISH2_CONFIG_DUMP_CONFIG_HPP_INCLUDE
#define DISH2_CONFIG_DUMP_CONFIG_HPP_INCLUDE

#include "../../../third-party/Empirical/include/emp/tools/keyname_utils.hpp"

#include "cfg.hpp"

namespace dish2 {

void dump_config() {

  auto keyname_attributes = emp::keyname::unpack_t{
    {"a", "asconfigured"},
    {"source", EMP_STRINGIFY(DISHTINY_HASH_)},
    {"ext", ".cfg"}
  };

  if ( std::getenv("REPRO_ID") ) {
    keyname_attributes[ "repro" ] = std::getenv("REPRO_ID");
  }

  const std::string outfilename = emp::keyname::pack( keyname_attributes );

  dish2::cfg.Write( outfilename );

}

} // namespace dish2

#endif // #ifndef DISH2_CONFIG_DUMP_CONFIG_HPP_INCLUDE
