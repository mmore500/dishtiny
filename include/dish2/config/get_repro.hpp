#pragma once
#ifndef DISH2_CONFIG_GET_REPRO_HPP_INCLUDE
#define DISH2_CONFIG_GET_REPRO_HPP_INCLUDE

#include <cstdlib>
#include <string>

#include "../../../third-party/Empirical/include/emp/base/optional.hpp"

#include "../config/cfg.hpp"

namespace dish2 {

emp::optional<std::string> get_repro() {

  if ( std::getenv("REPRO_ID") ) return std::string( std::getenv("REPRO_ID") );
  else return std::nullopt;

}

} // namespace dish2

#endif // #ifndef DISH2_CONFIG_GET_REPRO_HPP_INCLUDE
