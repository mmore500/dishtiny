#pragma once
#ifndef DISH2_CONFIGBYROOT_ROOT_MUTATION_CONFIGS_HPP_INCLUDE
#define DISH2_CONFIGBYROOT_ROOT_MUTATION_CONFIGS_HPP_INCLUDE

#include "RootMutationConfigs.hpp"

namespace dish2 {

  thread_local dish2::RootMutationConfigs root_mutation_configs{};

} // namespace dish2

#endif // #ifndef DISH2_CONFIGBYROOT_ROOT_MUTATION_CONFIGS_HPP_INCLUDE
