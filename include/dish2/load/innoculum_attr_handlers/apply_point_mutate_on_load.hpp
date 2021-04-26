#pragma once
#ifndef DISH2_LOAD_INNOCULUM_ATTR_HANDLERS_APPLY_POINT_MUTATE_ON_LOAD_HPP_INCLUDE
#define DISH2_LOAD_INNOCULUM_ATTR_HANDLERS_APPLY_POINT_MUTATE_ON_LOAD_HPP_INCLUDE

#include <utility>

#include "../../../../third-party/conduit/include/uitsl/math/math_utils.hpp"
#include "../../../../third-party/conduit/include/uitsl/polyfill/filesystem.hpp"
#include "../../../../third-party/Empirical/include/emp/tools/keyname_utils.hpp"

#include "../../debug/log_msg.hpp"
#include "../../genome/Genome.hpp"

namespace dish2 {

template<typename Spec>
void apply_point_mutate_on_load(
  dish2::Genome<Spec>& innoculum, const std::filesystem::path& path
) {

  thread_local std::filesystem::path last_path;
  const bool should_announce = path != std::exchange( last_path, path );

  const auto attrs = emp::keyname::unpack( path );

  if ( attrs.count("point_mutate_on_load") ) {
    const size_t num_muts = uitsl::stoszt( attrs.at("point_mutate_on_load") );
    if ( should_announce ) dish2::log_msg(
      "applying ", num_muts, " point mutations to genome ", attrs.at("root_id"),
      " from ", path
    );

    for (size_t i{}; i < num_muts; ++i) innoculum.DoPointMutation();
  }

}

} // namespace dish2

#endif // #ifndef DISH2_LOAD_INNOCULUM_ATTR_HANDLERS_APPLY_POINT_MUTATE_ON_LOAD_HPP_INCLUDE
