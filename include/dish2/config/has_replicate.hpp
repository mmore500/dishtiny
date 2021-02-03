#pragma once
#ifndef DISH2_CONFIG_HAS_REPLICATE_HPP_INCLUDE
#define DISH2_CONFIG_HAS_REPLICATE_HPP_INCLUDE

#include <limits>

#include "../config/cfg.hpp"

namespace dish2 {

bool has_replicate() {

  return !cfg.REPLICATE().empty();

}

} // namespace dish2

#endif // #ifndef DISH2_CONFIG_HAS_REPLICATE_HPP_INCLUDE
