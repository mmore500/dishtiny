#pragma once
#ifndef DISH2_UTILITY_MEASURE_COMPRESSION_RATIO_HPP_INCLUDE
#define DISH2_UTILITY_MEASURE_COMPRESSION_RATIO_HPP_INCLUDE

#include <fstream>

#include "../../../third-party/conduit/include/uitsl/fetch/make_temp_filepath.hpp"
#include "../../../third-party/conduit/include/uitsl/polyfill/filesystem.hpp"
#include "../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../third-party/header-only-gzstream/include/hogzstr/gzstream.hpp"

namespace dish2 {

template< typename T >
double measure_compression_ratio(const T& val) {

  // make temp filepath is expensive, so reuse temp paths
  thread_local const std::string uncompressed_path
    = uitsl::make_temp_filepath();
  thread_local const std::string compressed_path
    = uitsl::make_temp_filepath();

  // will overwrite
  {
    std::ofstream uncompressed_handle{ uncompressed_path };
    uncompressed_handle << val << std::flush;

    hogzstr::ogzstream compressed_handle{ compressed_path };
    compressed_handle << val << std::flush;
  }

  return (
    static_cast<double>( std::filesystem::file_size( compressed_path ) )
    / std::filesystem::file_size( uncompressed_path )
  );

}

} // namespace dish2

#endif // #ifndef DISH2_UTILITY_MEASURE_COMPRESSION_RATIO_HPP_INCLUDE
