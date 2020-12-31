#pragma once
#ifndef DISH2_CONFIG_TEMPORARYCONFIGOVERRIDE_HPP_INCLUDE
#define DISH2_CONFIG_TEMPORARYCONFIGOVERRIDE_HPP_INCLUDE

#include <string>

#include "../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "cfg.hpp"

namespace dish2 {

class TemporaryConfigOverride {

  std::string config_name;
  std::string config_bak;

public:

  template<typename T>
  TemporaryConfigOverride(const std::string& name, const T& temporary_override)
  : config_name( name )
  , config_bak( dish2::cfg( config_name ) )
  {

    dish2::cfg.Set( config_name, emp::to_string(temporary_override) );

  }

  ~TemporaryConfigOverride() { dish2::cfg.Set( config_name, config_bak ); }

};

} // namespace dish2

#endif // #ifndef DISH2_CONFIG_TEMPORARYCONFIGOVERRIDE_HPP_INCLUDE
