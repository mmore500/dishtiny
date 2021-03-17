#pragma once
#ifndef DISH2_CONFIGBYROOT_SUMMARIZE_ROOT_CONFIG_HPP_INCLUDE
#define DISH2_CONFIGBYROOT_SUMMARIZE_ROOT_CONFIG_HPP_INCLUDE

#include <map>
#include <string>

#include "../../../third-party/Empirical/include/emp/base/assert.hpp"

#include "root_mutation_configs.hpp"
#include "root_perturbation_configs.hpp"

namespace dish2 {

std::map<std::string, std::string> summarize_root_config(
  const size_t root_id
) {

  std::map<std::string, std::string> res;

  auto mutation_summary = dish2::root_mutation_configs.View(
    root_id
  ).MakeSummary();
  res.merge( mutation_summary );
  emp_assert( mutation_summary.empty() );

  auto perturbation_summary = dish2::root_perturbation_configs.View(
    root_id
  ).MakeSummary();
  res.merge( perturbation_summary );
  emp_assert( perturbation_summary.empty() );

  return res;

}

} // namespace dish2

#endif // #ifndef DISH2_CONFIGBYROOT_SUMMARIZE_ROOT_CONFIG_HPP_INCLUDE
