#pragma once
#ifndef DISH2_RECORD_DUMP_GENOME_STATISTICS_HPP_INCLUDE
#define DISH2_RECORD_DUMP_GENOME_STATISTICS_HPP_INCLUDE

#include <fstream>
#include <sstream>
#include <string>

#include "../../../third-party/cereal/include/cereal/archives/binary.hpp"
#include "../../../third-party/conduit/include/uitsl/algorithm/for_each.hpp"
#include "../../../third-party/conduit/include/uitsl/meta/function_cast.hpp"
#include "../../../third-party/conduit/include/uitsl/mpi/comm_utils.hpp"
#include "../../../third-party/conduit/include/uitsl/utility/keyname_key_union.hpp"
#include "../../../third-party/Empirical/include/emp/data/DataFile.hpp"
#include "../../../third-party/Empirical/include/emp/tools/keyname_utils.hpp"
#include "../../../third-party/Empirical/include/emp/tools/string_utils.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/introspection/count_instructions.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/introspection/count_modules.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/utility/CountingIterator.hpp"

#include "../config/has_replicate.hpp"
#include "../config/has_series.hpp"
#include "../config/has_stint.hpp"
#include "../genome/Genome.hpp"
#include "../load/make_genome_slug.hpp"
#include "../utility/measure_compression_ratio.hpp"
#include "../utility/pare_keyname_filename.hpp"

#include "dump_genome_statistics.hpp"

#include "make_filename/make_data_path.hpp"
#include "make_filename/make_genome_statistics_filename.hpp"

namespace dish2 {

template< typename Spec >
void dump_genome_statistics(
  const dish2::Genome< Spec >& genome, const std::string& genome_filename
) {

  using sgpl_spec_t = typename Spec::sgpl_spec_t;

  const std::string out_filename = dish2::pare_keyname_filename(
    dish2::make_genome_statistics_filename( genome_filename ),
    dish2::make_data_path()
  );

  emp::DataFile file( dish2::make_data_path( out_filename ) );

  if ( dish2::has_stint() ) file.AddVal(cfg.STINT(), "Stint");
  if ( dish2::has_series() ) file.AddVal(cfg.SERIES(), "Series");
  if ( dish2::has_replicate() ) file.AddVal(cfg.REPLICATE(), "Replicate");
  file.AddVal(cfg.TREATMENT(), "Treatment");
  if ( cfg.TREATMENT().find('=') != std::string::npos ) {
    for ( const auto& [k, v] : emp::keyname::unpack( cfg.TREATMENT() ) ) {
      file.AddVal( v, emp::to_string("Treatment ", k) );
    }
  }
  file.AddVal( "proc", emp::to_string( uitsl::get_proc_id() ) );

  const std::string slug = dish2::make_genome_slug( genome_filename );
  file.AddVar(slug, "Genome Slug");

  file.AddVar(genome_filename, "Genome Filename");

  // instruction category counts
  const size_t num_anchor_insts = sgpl::count_instructions<sgpl_spec_t>(
    genome.program, "anchor"
  );
  file.AddVar(num_anchor_insts, "Num Anchor Instructions");

  const size_t num_global_anchor_insts = sgpl::count_instructions<sgpl_spec_t>(
    genome.program, "global anchor"
  );
  file.AddVar(num_global_anchor_insts, "Num Global Anchor Instructions");

  const size_t num_local_anchor_insts = sgpl::count_instructions<sgpl_spec_t>(
    genome.program, "local anchor"
  );
  file.AddVar(num_local_anchor_insts, "Num Local Anchor Instructions");

  const size_t num_bitwise_insts = sgpl::count_instructions<sgpl_spec_t>(
    genome.program, "bitwise"
  );
  file.AddVar(num_bitwise_insts, "Num Bitwise Instructions");

  const size_t num_intermessaging_insts = sgpl::count_instructions<sgpl_spec_t>(
    genome.program, "intermessaging"
  );
  file.AddVar(num_intermessaging_insts, "Num Intermessaging Instructions");

  const size_t num_intrinsic_insts = sgpl::count_instructions<sgpl_spec_t>(
    genome.program, "intrinsic"
  );
  file.AddVar(num_intrinsic_insts, "Num Intrinsic Instructions");

  const size_t num_extrinsic_insts = sgpl::count_instructions<sgpl_spec_t>(
    genome.program, "extrinsic"
  );
  file.AddVar(num_extrinsic_insts, "Num Extrinsic Instructions");

  const size_t num_flow_insts = sgpl::count_instructions<sgpl_spec_t>(
    genome.program, "flow"
  );
  file.AddVar(num_flow_insts, "Num Flow Instructions");

  const size_t num_global_flow_insts = sgpl::count_instructions<sgpl_spec_t>(
    genome.program, "global flow"
  );
  file.AddVar(num_global_flow_insts, "Num Global Flow Instructions");

  const size_t num_local_flow_insts = sgpl::count_instructions<sgpl_spec_t>(
    genome.program, "local flow"
  );
  file.AddVar(num_local_flow_insts, "Num Local Flow Instructions");


  const size_t num_nop_insts = sgpl::count_instructions<sgpl_spec_t>(
    genome.program, "nop"
  );
  file.AddVar(num_nop_insts, "Num Nop Instructions");

  const size_t num_op_insts = sgpl::count_instructions<sgpl_spec_t>(
    genome.program, "op"
  );
  file.AddVar(num_op_insts, "Num Op Instructions");

  const size_t num_actuator_insts = sgpl::count_instructions<sgpl_spec_t>(
    genome.program, "actuator"
  );
  file.AddVar(num_actuator_insts, "Num Actuator Instructions");

  const size_t num_calculation_insts = sgpl::count_instructions<sgpl_spec_t>(
    genome.program, "calculation"
  );
  file.AddVar(num_calculation_insts, "Num Calculation Instructions");

  const size_t num_regulation_insts = sgpl::count_instructions<sgpl_spec_t>(
    genome.program, "regulation"
  );
  file.AddVar(num_regulation_insts, "Num Regulation Instructions");

  const size_t num_global_regulation_insts
    = sgpl::count_instructions<sgpl_spec_t>(
      genome.program, "global regulation"
    );
  file.AddVar(
    num_global_regulation_insts, "Num Global Regulation Instructions"
  );

  const size_t num_local_regulation_insts
    = sgpl::count_instructions<sgpl_spec_t>(genome.program, "local regulation");
  file.AddVar(num_local_regulation_insts, "Num Local Regulation Instructions");

  // other program attributes
  const double program_compression_ratio = dish2::measure_compression_ratio(
    [&](){
      std::ostringstream ss;
      cereal::BinaryOutputArchive archive( ss );
      archive( genome.program );
      return ss.str();
    }()
  );
  file.AddVar(program_compression_ratio, "Program Compression Ratio");

  const size_t num_instructions = genome.program.size();
  file.AddVar(num_instructions, "Num Instructions");

  const size_t num_modules = sgpl::count_modules<sgpl_spec_t>(
    genome.program
  );
  file.AddVar(num_modules, "Num Modules");

  const double coding_fraction
    = static_cast<double>( num_op_insts ) / num_instructions;
  file.AddVar(coding_fraction, "Coding Fraction");

  const double noncoding_fraction
    = static_cast<double>( num_nop_insts ) / num_instructions;
  file.AddVar(noncoding_fraction, "Non-coding Fraction");

  emp_assert( num_op_insts + num_nop_insts == num_instructions );

  // phylogenetic attributes
  uitsl::for_each(
    std::begin( genome.generation_counter.elapsed_generations ),
    std::end(genome.generation_counter.elapsed_generations ),
    sgpl::CountingIterator{},
    [&file]( const auto& generation, const size_t i ){
      file.AddVar( generation, emp::to_string("Elapsed Generations Level ", i) );
    }
  );

  const size_t elapsed_indel_mutations
    = genome.mutation_counter.insertion_deletion_counter;
  file.AddVar(elapsed_indel_mutations, "Elapsed Indel Mutations");

  const size_t elapsed_point_mutations
    = genome.mutation_counter.point_mutation_counter;
  file.AddVar(elapsed_point_mutations, "Elapsed Point Mutations");

  const size_t elapsed_mutations
    = genome.mutation_counter.mutation_occurrence_counter;
  file.AddVar(elapsed_mutations, "Elapsed Mutations");

  for (
    const auto& key
    : uitsl::keyname_key_union({ genome_filename })
  ) file.AddFun( uitsl::function_cast( [key, &genome_filename](){
    const auto attrs = emp::keyname::unpack( genome_filename );
    const auto res = attrs.find( key );
    return res != std::end( attrs ) ? res->second : "";
  } ), emp::to_string("genome ", key ));


  file.PrintHeaderKeys();
  file.Update();

}

} // namespace dish2

#endif // #ifndef DISH2_RECORD_DUMP_GENOME_STATISTICS_HPP_INCLUDE
