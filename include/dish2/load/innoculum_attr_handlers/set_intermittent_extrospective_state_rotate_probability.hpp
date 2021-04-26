#pragma once
#ifndef DISH2_LOAD_INNOCULUM_ATTR_HANDLERS_SET_INTERMITTENT_EXTROSPECTIVE_STATE_ROTATE_PROBABILITY_HPP_INCLUDE
#define DISH2_LOAD_INNOCULUM_ATTR_HANDLERS_SET_INTERMITTENT_EXTROSPECTIVE_STATE_ROTATE_PROBABILITY_HPP_INCLUDE

#include <string>
#include <utility>

#include "../../../../third-party/conduit/include/uitsl/polyfill/filesystem.hpp"
#include "../../../../third-party/Empirical/include/emp/tools/keyname_utils.hpp"

#include "../../configbyroot/root_perturbation_configs.hpp"
#include "../../debug/log_msg.hpp"

namespace dish2 {

template<typename Spec>
void set_intermittent_extrospective_state_rotate_probability(
  const std::filesystem::path& path
) {

  const auto attrs = emp::keyname::unpack( path );

  const size_t root_id = uitsl::stoszt( attrs.at("root_id") );

  if ( attrs.count("set_intermittent_extrospective_state_rotate_probability") ) {
    const double intermittent_extrospective_state_rotate_probability
      = std::stod( attrs.at("set_intermittent_extrospective_state_rotate_probability") );
    dish2::log_msg(
      " setting intermittent_extrospective_state_rotate_probability to ",
      intermittent_extrospective_state_rotate_probability,
      " for root id ", root_id, " from ", path
    );

    dish2::root_perturbation_configs.Get(
      root_id
    ).intermittent_extrospective_state_rotate_probability
      = intermittent_extrospective_state_rotate_probability;
  }

}

} // namespace dish2

#endif // #ifndef DISH2_LOAD_INNOCULUM_ATTR_HANDLERS_SET_INTERMITTENT_EXTROSPECTIVE_STATE_ROTATE_PROBABILITY_HPP_INCLUDE
