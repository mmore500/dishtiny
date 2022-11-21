#pragma once
#ifndef DISH2_RECORD_MAKE_FILENAME_MAKE_DUMP_ABUNDANCE_GENOME_FILENAME_HPP_INCLUDE
#define DISH2_RECORD_MAKE_FILENAME_MAKE_DUMP_ABUNDANCE_GENOME_FILENAME_HPP_INCLUDE

#include <cstddef>
#include <string>

#include "make_dump_abundance_lowest_root_genome_filename.hpp"

namespace dish2 {

[[deprecated]]
std::string make_dump_abundance_genome_filename(
  const size_t count,
  const double abundance,
  const double prevalence,
  const std::string& morph,
  const size_t background_population_size = 0
) {
  return dish2::make_dump_abundance_lowest_root_genome_filename(
    count,
    abundance,
    prevalence,
    morph,
    background_population_size
  );
}

} // namespace dish2

#endif // #ifndef DISH2_RECORD_MAKE_FILENAME_MAKE_DUMP_ABUNDANCE_GENOME_FILENAME_HPP_INCLUDE
