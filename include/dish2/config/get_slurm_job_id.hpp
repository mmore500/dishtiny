#pragma once
#ifndef DISH2_CONFIG_GET_SLURM_JOB_ID_HPP_INCLUDE
#define DISH2_CONFIG_GET_SLURM_JOB_ID_HPP_INCLUDE

#include <cstdlib>
#include <string>

#include "../../../third-party/Empirical/include/emp/base/optional.hpp"

namespace dish2 {

emp::optional<std::string> get_slurm_job_id() {

  if ( std::getenv("SLURM_JOB_ID") ) return std::string(
    std::getenv("SLURM_JOB_ID")
  );
  else return std::nullopt;

}

} // namespace dish2

#endif // #ifndef DISH2_CONFIG_GET_SLURM_JOB_ID_HPP_INCLUDE
