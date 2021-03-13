#pragma once
#ifndef DISH2_CONFIGBYROOT_ROOT_PERTURBATION_CONFIGS_HPP_INCLUDE
#define DISH2_CONFIGBYROOT_ROOT_PERTURBATION_CONFIGS_HPP_INCLUDE

#include "RootPerturbationConfigs.hpp"

namespace dish2 {

  thread_local dish2::RootPerturbationConfigs root_perturbation_configs{};

} // namespace dish2

#endif // #ifndef DISH2_CONFIGBYROOT_ROOT_PERTURBATION_CONFIGS_HPP_INCLUDE
