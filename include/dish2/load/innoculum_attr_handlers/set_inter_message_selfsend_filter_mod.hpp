#pragma once
#ifndef DISH2_LOAD_INNOCULUM_ATTR_HANDLERS_SET_INTER_MESSAGE_SELFSEND_FILTER_MOD_HPP_INCLUDE
#define DISH2_LOAD_INNOCULUM_ATTR_HANDLERS_SET_INTER_MESSAGE_SELFSEND_FILTER_MOD_HPP_INCLUDE

#include <utility>

#include "../../../../third-party/conduit/include/uitsl/math/math_utils.hpp"
#include "../../../../third-party/conduit/include/uitsl/polyfill/filesystem.hpp"
#include "../../../../third-party/Empirical/include/emp/tools/keyname_utils.hpp"

#include "../../configbyroot/root_perturbation_configs.hpp"
#include "../../debug/log_msg.hpp"

namespace dish2 {

template<typename Spec>
void set_inter_message_selfsend_filter_mod(const std::filesystem::path& path) {

  const auto attrs = emp::keyname::unpack( path );

  const size_t root_id = uitsl::stoszt( attrs.at("root_id") );

  if ( attrs.count("set_inter_message_selfsend_filter_mod") ) {
    const size_t inter_message_selfsend_filter_mod = uitsl::stoszt(
      attrs.at("set_inter_message_selfsend_filter_mod")
    );
    dish2::log_msg(
      "setting inter_message_selfsend_filter_mod to ",
      inter_message_selfsend_filter_mod,
      " for root id ", root_id, " from ", path
    );
    dish2::root_perturbation_configs.Get(
      root_id
    ).inter_message_selfsend_filter_mod
      = inter_message_selfsend_filter_mod;
  }

}

} // namespace dish2

#endif // #ifndef DISH2_LOAD_INNOCULUM_ATTR_HANDLERS_SET_INTER_MESSAGE_SELFSEND_FILTER_MOD_HPP_INCLUDE
