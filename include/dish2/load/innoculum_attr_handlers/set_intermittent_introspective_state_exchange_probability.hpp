#pragma once
#ifndef DISH2_LOAD_INNOCULUM_ATTR_HANDLERS_SET_INTERMITTENT_INTROSPECTIVE_STATE_EXCHANGE_PROBABILITY_HPP_INCLUDE
#define DISH2_LOAD_INNOCULUM_ATTR_HANDLERS_SET_INTERMITTENT_INTROSPECTIVE_STATE_EXCHANGE_PROBABILITY_HPP_INCLUDE

#include <string>
#include <utility>

#include "../../../../third-party/conduit/include/uitsl/mpi/comm_utils.hpp"
#include "../../../../third-party/conduit/include/uitsl/polyfill/filesystem.hpp"
#include "../../../../third-party/Empirical/include/emp/tools/keyname_utils.hpp"

#include "../../configbyroot/root_perturbation_configs.hpp"

namespace dish2 {

template<typename Spec>
void set_intermittent_introspective_state_exchange_probability(
  const std::filesystem::path& path, const size_t thread_idx
) {

  const auto attrs = emp::keyname::unpack( path );

  const size_t root_id = uitsl::stoszt( attrs.at("root_id") );

  if ( attrs.count("set_intermittent_introspective_state_exchange_probability") ) {
    const double intermittent_introspective_state_exchange_probability = std::stod(
      attrs.at("set_intermittent_introspective_state_exchange_probability")
    );
    std::cout  << "proc " << uitsl::get_proc_id() << " thread " << thread_idx
      << " setting intermittent_introspective_state_exchange_probability to "
      << intermittent_introspective_state_exchange_probability
      << " for root id " << root_id << " from " << path << std::endl;
    dish2::root_perturbation_configs.Get(
      root_id
    ).intermittent_introspective_state_exchange_probability
      = intermittent_introspective_state_exchange_probability;
  }

}

} // namespace dish2

#endif // #ifndef DISH2_LOAD_INNOCULUM_ATTR_HANDLERS_SET_INTERMITTENT_INTROSPECTIVE_STATE_EXCHANGE_PROBABILITY_HPP_INCLUDE
