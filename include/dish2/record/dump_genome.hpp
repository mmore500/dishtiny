#pragma once
#ifndef DISH2_RECORD_DUMP_GENOME_HPP_INCLUDE
#define DISH2_RECORD_DUMP_GENOME_HPP_INCLUDE

#include <fstream>
#include <string>

#include "../../../third-party/cereal/include/cereal/archives/json.hpp"
#include "../../../third-party/Empirical/include/emp/tools/keyname_utils.hpp"

#include "../genome/Genome.hpp"

#include "dump_genome_statistics.hpp"

namespace dish2 {

template< typename Spec >
void dump_genome(
  const dish2::Genome< Spec >& genome, const std::string& genome_filename
) {

  {
    std::ofstream os( genome_filename );
    cereal::JSONOutputArchive archive( os );
    archive( genome );
  }

  dish2::dump_genome_statistics<Spec>(
    genome, genome_filename
  );

}

} // namespace dish2

#endif // #ifndef DISH2_RECORD_DUMP_GENOME_HPP_INCLUDE
