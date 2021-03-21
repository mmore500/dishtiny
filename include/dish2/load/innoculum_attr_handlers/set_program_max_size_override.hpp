#pragma once
#ifndef DISH2_LOAD_INNOCULUM_ATTR_HANDLERS_SET_PROGRAM_MAX_SIZE_OVERRIDE_HPP_INCLUDE
#define DISH2_LOAD_INNOCULUM_ATTR_HANDLERS_SET_PROGRAM_MAX_SIZE_OVERRIDE_HPP_INCLUDE

#include <utility>

#include "../../../../third-party/conduit/include/uitsl/mpi/comm_utils.hpp"
#include "../../../../third-party/conduit/include/uitsl/polyfill/filesystem.hpp"
#include "../../../../third-party/Empirical/include/emp/tools/keyname_utils.hpp"

#include "../../configbyroot/root_mutation_configs.hpp"

namespace dish2 {

template<typename Spec>
void set_program_max_size_override( const std::filesystem::path& path ) {

  const auto attrs = emp::keyname::unpack( path );

  const size_t root_id = uitsl::stoszt( attrs.at("root_id") );

  if ( attrs.count("set_program_max_size_override") ) {
    const size_t program_max_size_override
      = uitsl::stoszt( attrs.at("set_program_max_size_override") );
    std::cout  << "proc " << uitsl::get_proc_id()
      << " thread " << dish2::thread_idx
      << " setting program_max_size_override to "
      << program_max_size_override
      << " for root id " << root_id << " from " << path << std::endl;

    dish2::root_mutation_configs.Get(
      root_id
    ).program_max_size_override = program_max_size_override;
  }

}

} // namespace dish2

#endif // #ifndef DISH2_LOAD_INNOCULUM_ATTR_HANDLERS_SET_PROGRAM_MAX_SIZE_OVERRIDE_HPP_INCLUDE
