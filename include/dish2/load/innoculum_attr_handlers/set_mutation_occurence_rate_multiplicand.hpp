#pragma once
#ifndef DISH2_LOAD_INNOCULUM_ATTR_HANDLERS_SET_MUTATION_OCCURENCE_RATE_MULTIPLICAND_HPP_INCLUDE
#define DISH2_LOAD_INNOCULUM_ATTR_HANDLERS_SET_MUTATION_OCCURENCE_RATE_MULTIPLICAND_HPP_INCLUDE

#include <string>
#include <utility>

#include "../../../../third-party/conduit/include/uitsl/polyfill/filesystem.hpp"
#include "../../../../third-party/Empirical/include/emp/tools/keyname_utils.hpp"

#include "../../configbyroot/root_mutation_configs.hpp"
#include "../../debug/log_msg.hpp"

namespace dish2 {

template<typename Spec>
void set_mutation_occurence_rate_multiplicand(
  const std::filesystem::path& path
) {

  const auto attrs = emp::keyname::unpack( path );

  const size_t root_id = uitsl::stoszt( attrs.at("root_id") );

  if ( attrs.count("set_mutation_occurence_rate_multiplicand") ) {
    const double mutation_occurence_rate_multiplicand
      = std::stod( attrs.at("set_mutation_occurence_rate_multiplicand") );
    dish2::log_msg(
      "setting mutation_occurence_rate_multiplicand to ",
      mutation_occurence_rate_multiplicand,
      " for root id ", root_id, " from ", path
    );

    dish2::root_mutation_configs.Get(
      root_id
    ).mutation_occurence_rate_multiplicand
      = mutation_occurence_rate_multiplicand;
  }

}

} // namespace dish2

#endif // #ifndef DISH2_LOAD_INNOCULUM_ATTR_HANDLERS_SET_MUTATION_OCCURENCE_RATE_MULTIPLICAND_HPP_INCLUDE
