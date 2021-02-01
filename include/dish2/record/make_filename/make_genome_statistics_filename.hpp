#pragma once
#ifndef DISH2_RECORD_MAKE_FILENAME_MAKE_GENOME_STATISTICS_FILENAME_HPP_INCLUDE
#define DISH2_RECORD_MAKE_FILENAME_MAKE_GENOME_STATISTICS_FILENAME_HPP_INCLUDE

#include <string>

#include "../../../../third-party/Empirical/include/emp/tools/keyname_utils.hpp"

namespace dish2 {

std::string make_genome_statistics_filename( const std::string& genome_fname ) {

  auto statistics_attrs = emp::keyname::unpack( genome_fname );
  statistics_attrs["a"] = "genome_statistics";
  statistics_attrs["ext"] = ".csv";

  return emp::keyname::pack( statistics_attrs );

}

} // namespace dish2

#endif // #ifndef DISH2_RECORD_MAKE_FILENAME_MAKE_GENOME_STATISTICS_FILENAME_HPP_INCLUDE
