#pragma once
#ifndef DISH2_CONFIG_DUMP_CONFIG_HPP_INCLUDE
#define DISH2_CONFIG_DUMP_CONFIG_HPP_INCLUDE

#include <string>

#include "../../../third-party/conduit/include/uitsl/mpi/comm_utils.hpp"
#include "../../../third-party/Empirical/include/emp/base/macros.hpp"
#include "../../../third-party/Empirical/include/emp/tools/keyname_utils.hpp"
#include "../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "../utility/mkdir_exists_ok.hpp"

#include "cfg.hpp"

namespace dish2 {

void dump_config() {

  auto keyname_attributes = emp::keyname::unpack_t{
    {"a", "asconfigured"},
    {"proc", emp::to_string( uitsl::get_proc_id() ) },
    {"source", EMP_STRINGIFY(DISHTINY_HASH_)},
    {"ext", ".cfg"}
  };

  if ( std::getenv("REPRO_ID") ) {
    keyname_attributes[ "repro" ] = std::getenv("REPRO_ID");
  }

  const std::string outpath
    = std::string("outmeta/") + emp::keyname::pack( keyname_attributes );

  dish2::mkdir_exists_ok( "./outmeta" );
  dish2::cfg.Write( outpath );

}

} // namespace dish2

#endif // #ifndef DISH2_CONFIG_DUMP_CONFIG_HPP_INCLUDE
