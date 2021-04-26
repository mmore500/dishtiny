#pragma once
#ifndef DISH2_LOAD_INNOCULUM_ATTR_HANDLERS_SET_WRITABLE_STATE_TARGET_IDX_HPP_INCLUDE
#define DISH2_LOAD_INNOCULUM_ATTR_HANDLERS_SET_WRITABLE_STATE_TARGET_IDX_HPP_INCLUDE

#include <optional>
#include <utility>

#include "../../../../third-party/conduit/include/uitsl/polyfill/filesystem.hpp"
#include "../../../../third-party/Empirical/include/emp/base/optional.hpp"
#include "../../../../third-party/Empirical/include/emp/tools/keyname_utils.hpp"
#include "../../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "../../configbyroot/root_perturbation_configs.hpp"
#include "../../debug/log_msg.hpp"

namespace dish2 {

template<typename Spec>
void set_writable_state_target_idx( const std::filesystem::path& path ) {

  const auto attrs = emp::keyname::unpack( path );

  const size_t root_id = uitsl::stoszt( attrs.at("root_id") );

  if ( attrs.count("set_writable_state_target_idx") ) {

    using idx_t = emp::optional<size_t>;
    const idx_t writable_state_target_idx =
      attrs.at("set_writable_state_target_idx").empty()
      ? std::nullopt
      : idx_t{ uitsl::stoszt(attrs.at("set_writable_state_target_idx")) }
    ;

    dish2::log_msg(
      "setting writable_state_target_idx to \"", writable_state_target_idx,
      "\" for root id ", root_id, " from ", path
    );

    dish2::root_perturbation_configs.Get(
      root_id
    ).writable_state_target_idx = writable_state_target_idx;
  }

}

} // namespace dish2

#endif // #ifndef DISH2_LOAD_INNOCULUM_ATTR_HANDLERS_SET_WRITABLE_STATE_TARGET_IDX_HPP_INCLUDE
