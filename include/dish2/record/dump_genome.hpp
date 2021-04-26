#pragma once
#ifndef DISH2_RECORD_DUMP_GENOME_HPP_INCLUDE
#define DISH2_RECORD_DUMP_GENOME_HPP_INCLUDE

#include <fstream>
#include <string>

#include "../../../third-party/cereal/include/cereal/archives/json.hpp"
#include "../../../third-party/Empirical/include/emp/tools/keyname_utils.hpp"
#include "../../../third-party/header-only-gzstream/include/hogzstr/gzstream.hpp"

#include "../genome/Genome.hpp"
#include "../utility/pare_keyname_filename.hpp"

#include "dump_genome_statistics.hpp"
#include "make_filename/make_artifact_path.hpp"

namespace dish2 {

template< typename Spec >
void dump_genome(
  const dish2::Genome< Spec >& genome, const std::string& genome_filename
) {

  const auto attrs = emp::keyname::unpack( genome_filename );

  const std::string out_filename = dish2::pare_keyname_filename(
    genome_filename,
    dish2::make_artifact_path()
  );

  if ( attrs.count( "ext" ) && attrs.at( "ext" ) == ".json" ) {
    std::ofstream os( dish2::make_artifact_path( out_filename ) );
    cereal::JSONOutputArchive archive( os );
    archive( genome );
  } else if ( attrs.count( "ext" ) && attrs.at( "ext" ) == ".json.gz" ) {
    hogzstr::ogzstream os( dish2::make_artifact_path( out_filename ) );
    cereal::JSONOutputArchive archive( os );
    archive( genome );
  } else { emp_always_assert( false, genome_filename ); }

  dish2::dump_genome_statistics<Spec>(
    genome, genome_filename
  );

}

} // namespace dish2

#endif // #ifndef DISH2_RECORD_DUMP_GENOME_HPP_INCLUDE
