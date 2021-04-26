#pragma once
#ifndef DISH2_RECORD_WRITE_DEMOGRAPHIC_PHENOTYPIC_PHYLOGENETIC_METRICS_HPP_INCLUDE
#define DISH2_RECORD_WRITE_DEMOGRAPHIC_PHENOTYPIC_PHYLOGENETIC_METRICS_HPP_INCLUDE

#include <mutex>

#include "../../../third-party/conduit/include/uitsl/mpi/comm_utils.hpp"
#include "../../../third-party/Empirical/include/emp/base/macros.hpp"
#include "../../../third-party/Empirical/include/emp/data/DataFile.hpp"
#include "../../../third-party/Empirical/include/emp/tools/keyname_utils.hpp"
#include "../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "../config/cfg.hpp"
#include "../config/has_replicate.hpp"
#include "../config/has_series.hpp"
#include "../config/has_stint.hpp"
#include "../config/thread_idx.hpp"
#include "../debug/log_msg.hpp"
#include "../introspection/any_live_cells.hpp"
#include "../introspection/count_birth_events.hpp"
#include "../introspection/count_cardinals.hpp"
#include "../introspection/count_dead_cells.hpp"
#include "../introspection/count_death_events.hpp"
#include "../introspection/count_kin_neighbors.hpp"
#include "../introspection/count_live_cardinals.hpp"
#include "../introspection/count_live_cells.hpp"
#include "../introspection/count_spawn_events.hpp"
#include "../introspection/count_unique_coding_genotypes.hpp"
#include "../introspection/count_unique_module_expression_profiles.hpp"
#include "../introspection/count_unique_module_regulation_profiles.hpp"
#include "../introspection/count_unique_root_ids.hpp"
#include "../introspection/count_unique_stint_root_ids.hpp"
#include "../introspection/get_fraction_cardinals_apoptosis_request.hpp"
#include "../introspection/get_fraction_cardinals_incoming_inter_message.hpp"
#include "../introspection/get_fraction_cardinals_incoming_intra_message.hpp"
#include "../introspection/get_fraction_cardinals_receiving_resource.hpp"
#include "../introspection/get_fraction_cardinals_resource_receive_resistance.hpp"
#include "../introspection/get_fraction_cardinals_resource_reserve_request.hpp"
#include "../introspection/get_fraction_cardinals_resource_send_request.hpp"
#include "../introspection/get_fraction_cardinals_spawn_arrest.hpp"
#include "../introspection/get_fraction_cardinals_spawn_request.hpp"
#include "../introspection/get_fraction_cells_apoptosis_request.hpp"
#include "../introspection/get_fraction_cells_incoming_inter_message.hpp"
#include "../introspection/get_fraction_cells_incoming_intra_message.hpp"
#include "../introspection/get_fraction_cells_kin_group_loners.hpp"
#include "../introspection/get_fraction_cells_receiving_resource.hpp"
#include "../introspection/get_fraction_cells_resource_receive_resistance.hpp"
#include "../introspection/get_fraction_cells_resource_reserve_request.hpp"
#include "../introspection/get_fraction_cells_resource_send_request.hpp"
#include "../introspection/get_fraction_cells_spawn_arrest.hpp"
#include "../introspection/get_fraction_cells_spawn_request.hpp"
#include "../introspection/get_fraction_fecund_resource_stockpile.hpp"
#include "../introspection/get_fraction_live_cells.hpp"
#include "../introspection/get_fraction_nulliparous.hpp"
#include "../introspection/get_maximum_kin_group_size.hpp"
#include "../introspection/get_mean_cell_age.hpp"
#include "../introspection/get_mean_elapsed_insertions_deletions.hpp"
#include "../introspection/get_mean_elapsed_mutation_occurences.hpp"
#include "../introspection/get_mean_elapsed_point_mutations.hpp"
#include "../introspection/get_mean_epoch.hpp"
#include "../introspection/get_mean_generation.hpp"
#include "../introspection/get_mean_genome_compression_ratio.hpp"
#include "../introspection/get_mean_incoming_inter_message_count_per_cardinal.hpp"
#include "../introspection/get_mean_incoming_inter_message_count_per_cell.hpp"
#include "../introspection/get_mean_incoming_intra_message_count_per_cardinal.hpp"
#include "../introspection/get_mean_incoming_intra_message_count_per_cell.hpp"
#include "../introspection/get_mean_kin_group_age.hpp"
#include "../introspection/get_mean_kin_group_size.hpp"
#include "../introspection/get_mean_module_count.hpp"
#include "../introspection/get_mean_program_length.hpp"
#include "../introspection/get_mean_resource_received_per_cardinal.hpp"
#include "../introspection/get_mean_resource_received_per_cell.hpp"
#include "../introspection/get_mean_resource_stockpile.hpp"
#include "../introspection/get_mean_spawn_count.hpp"
#include "../introspection/get_median_kin_group_size.hpp"
#include "../introspection/get_num_running_log_cardinal_updates.hpp"
#include "../introspection/get_num_running_log_cell_updates.hpp"
#include "../introspection/get_num_running_log_live_cardinal_updates.hpp"
#include "../introspection/get_num_running_log_live_cell_updates.hpp"
#include "../introspection/get_population_compression_ratio.hpp"
#include "../introspection/get_prevalent_coding_genotype.hpp"
#include "../introspection/make_causes_of_death_string_histogram.hpp"
#include "../introspection/sum_entire_elapsed_instruction_cycles_for_live_cells.hpp"
#include "../introspection/sum_entire_elapsed_instruction_cycles.hpp"
#include "../utility/pare_keyname_filename.hpp"

#include "make_filename/make_data_path.hpp"
#include "make_filename/make_demographic_phenotypic_phylogenetic_metrics_filename.hpp"

namespace dish2 {

template< typename Spec >
void write_demographic_phenotypic_phylogenetic_metrics(
  const dish2::ThreadWorld< Spec >& world
) {

  dish2::log_msg( "writing demographic phenotypic phylogenetic metrics" );

  const thread_local std::string out_filename = dish2::pare_keyname_filename(
    dish2::make_demographic_phenotypic_phylogenetic_metrics_filename(),
    dish2::make_data_path()
  );

  thread_local emp::DataFile file( dish2::make_data_path(
    out_filename
  ) );

  thread_local std::string metric;
  thread_local double value;
  thread_local size_t update;

  update = world.GetUpdate();

  thread_local std::once_flag once_flag;
  std::call_once(once_flag, [](){
    if ( dish2::has_stint() ) file.AddVal(cfg.STINT(), "Stint");
    if ( dish2::has_series() ) file.AddVal(cfg.SERIES(), "Series");
    if ( dish2::has_replicate() ) file.AddVal(cfg.REPLICATE(), "Replicate");
    file.AddVal(cfg.TREATMENT(), "Treatment");
    if ( cfg.TREATMENT().find('=') != std::string::npos ) {
      for ( const auto& [k, v] : emp::keyname::unpack( cfg.TREATMENT() ) ) {
        file.AddVal( v, emp::to_string("Treatment ", k) );
      }
    }
    file.AddVal( uitsl::get_proc_id(), "proc" );
    file.AddVal( dish2::thread_idx, "thread" );

    file.AddVar(metric, "Metric");
    file.AddVar(value, "Value");
    file.AddVar(update, "Update");
    file.PrintHeaderKeys();

  });



  {
    metric = "Number Cells";
    value = world.GetSize();
    file.Update();
  }

  {
    metric = "Number Cardinals";
    value = dish2::count_cardinals<Spec>( world );
    file.Update();
  }

  {
    metric = "Mean Current Epoch";
    value = dish2::get_mean_epoch<Spec>( world );
    file.Update();
  }

  {
    metric = "Random Number Generator Seed";
    value = sgpl::tlrand.Get().GetSeed();
    file.Update();
  }

  // PHYLOGENETIC METRICS

  {
    metric = "Number Phylogenetic Roots";
    value = dish2::count_unique_root_ids<Spec>( world );
    file.Update();
  }

  {
    metric = "Number Stint Phylogenetic Roots";
    value = dish2::count_unique_stint_root_ids<Spec>( world );
    file.Update();
  }

  {
    metric = "Mean Elapsed Indel Mutations";
    value = dish2::get_mean_elapsed_insertions_deletions<Spec>( world );
    file.Update();
  }

  {
    metric = "Mean Elapsed Point Mutations";
    value = dish2::get_mean_elapsed_point_mutations<Spec>( world );
    file.Update();
  }

  {
    metric = "Mean Elapsed Mutation Occurences";
    value = dish2::get_mean_elapsed_mutation_occurences<Spec>( world );
    file.Update();
  }

  for (size_t lev{}; lev <= Spec::NLEV; ++lev) {
    metric = emp::to_string("Mean Elapsed Generations Level ", lev);
    value = dish2::get_mean_generation<Spec>( world, lev );
    file.Update();
  }

  // DEMOGRAPHIC METRICS

  {
    metric = "Number Dead Cells";
    value = dish2::count_dead_cells<Spec>( world );
    file.Update();
  }

  {
    metric = "Number Live Cells";
    value = dish2::count_live_cells<Spec>( world );
    file.Update();
  }

  {
    metric = "Fraction Live Cells";
    value = dish2::get_fraction_live_cells<Spec>( world );
    file.Update();
  }

  {
    metric = "Extinct";
    value = !dish2::any_live_cells<Spec>( world );
    file.Update();
  }

  {
    metric = "Number Live Cardinals";
    value = dish2::count_live_cardinals<Spec>( world );
    file.Update();
  }

  {
    metric = "Number Unique Genotypes";
    value = dish2::count_unique_coding_genotypes<Spec>( world );
    file.Update();
  }

  // disabled because they seem to be very slow on the hpcc
  // {
  //   metric = "Population Compression Ratio";
  //   value = dish2::get_population_compression_ratio<Spec>( world );
  //   file.Update();
  // }
  //
  // {
  //   metric = "Mean Genome Compression Ratio";
  //   value = dish2::get_mean_genome_compression_ratio<Spec>( world );
  //   file.Update();
  // }

  {
    metric = "Mean Program Module Count";
    value = dish2::get_mean_module_count<Spec>( world );
    file.Update();
  }

  {
    metric = "Mean Program Instruction Count";
    value = dish2::get_mean_program_length<Spec>( world );
    file.Update();
  }

  {
    metric = "Prevalent Genotype Quantity";
    value = dish2::get_prevalent_coding_genotype<Spec>( world ).second;
    file.Update();
  }

  {
    metric = "Mean Cell Age";
    value = dish2::get_mean_cell_age<Spec>( world );
    file.Update();
  }

  for (size_t lev{}; lev < Spec::NLEV; ++lev) {
    metric = emp::to_string("Mean Kin Group Age Level ", lev);
    value = dish2::get_mean_kin_group_age<Spec>( world, lev );
    file.Update();
  }


  // PHENOTYPIC METRICS

  {
    metric = "Number Unique Module Regulation Profiles";
    value = dish2::count_unique_module_regulation_profiles<Spec>( world );
    file.Update();
  }

  {
    metric = "Number Unique Module Expression Profiles";
    value = dish2::count_unique_module_expression_profiles<Spec>( world );
    file.Update();
  }


  {
    metric = "Mean Resource Stockpile";
    value = dish2::get_mean_resource_stockpile<Spec>( world );
    file.Update();
  }

  {
    metric = "Fecund Resource Stockpile Fraction";
    value = dish2::get_fraction_fecund_resource_stockpile<Spec>( world );
    file.Update();
  }

  {
    metric = "Resource Receiving Cardinal Fraction";
    value = dish2::get_fraction_cardinals_receiving_resource<Spec>( world );
    file.Update();
  }


  {
    metric = "Resource Receiving Cell Fraction";
    value = dish2::get_fraction_cells_receiving_resource<Spec>( world );
    file.Update();
  }

  {
    metric = "Mean Resource Received Per Cardinal";
    value = dish2::get_mean_resource_received_per_cardinal<Spec>( world );
    file.Update();
  }

  {
    metric = "Mean Resource Received Per Cell";
    value = dish2::get_mean_resource_received_per_cell<Spec>( world );
    file.Update();
  }

  {
    metric = "Resource Send Request Cardinal Fraction";
    value = dish2::get_fraction_cardinals_resource_send_request<Spec>( world );
    file.Update();
  }

  {
    metric = "Resource Send Request Cell Fraction";
    value = dish2::get_fraction_cells_resource_send_request<Spec>( world );
    file.Update();
  }

  {
    metric = "Resource Reserve Request Cardinal Fraction";
    value = dish2::get_fraction_cardinals_resource_reserve_request<Spec>(
      world
    );
    file.Update();
  }


  {
    metric = "Resource Reserve Request Cell Fraction";
    value = dish2::get_fraction_cells_resource_reserve_request<Spec>(
      world
    );
    file.Update();
  }

  {
    metric = "Resource Receive Resistance Cardinal Fraction";
    value = dish2::get_fraction_cardinals_resource_recieve_resistance<Spec>(
      world
    );
    file.Update();
  }

  {
    metric = "Resource Receive Resistance Cell Fraction";
    value = dish2::get_fraction_cells_resource_recieve_resistance<Spec>(world);
    file.Update();
  }

  {
    metric = "Spawn Arrest Cardinal Fraction";
    value = dish2::get_fraction_cardinals_spawn_arrest<Spec>( world );
    file.Update();
  }

  {
    metric = "Spawn Arrest Cell Fraction";
    value = dish2::get_fraction_cells_spawn_arrest<Spec>( world );
    file.Update();
  }

  {
    metric = "Spawn Request Cardinal Fraction";
    value = dish2::get_fraction_cardinals_spawn_request<Spec>( world );
    file.Update();
  }

  {
    metric = "Spawn Request Cell Fraction";
    value = dish2::get_fraction_cells_spawn_request<Spec>( world );
    file.Update();
  }

  {
    metric = "Nulliparous Fraction";
    value = dish2::get_fraction_nulliparous<Spec>( world );
    file.Update();
  }

  {
    metric = "Mean Spawn Count";
    value = dish2::get_mean_spawn_count<Spec>( world );
    file.Update();
  }

  {
    metric = "Cardinal Apoptosis Request Fraction";
    value = dish2::get_fraction_cardinals_apoptosis_request<Spec>( world );
    file.Update();
  }

  {
    metric = "Cell Apoptosis Request Fraction";
    value = dish2::get_fraction_cells_apoptosis_request<Spec>( world );
    file.Update();
  }

  {
    metric = "Mean Incoming Intra Message Count Per Cardinal";
    value = dish2::get_mean_incoming_intra_message_count_per_cardinal<Spec>(
      world
    );
    file.Update();
  }

  {
    metric = "Mean Incoming Intra Message Count Per Cell";
    value = dish2::get_mean_incoming_intra_message_count_per_cell<Spec>(
      world
    );
    file.Update();
  }

  {
    metric = "Mean Incoming Inter Message Count Per Cardinal";
    value = dish2::get_mean_incoming_inter_message_count_per_cardinal<Spec>(
      world
    );
    file.Update();
  }

  {
    metric = "Mean Incoming Inter Message Count Per Cell";
    value = dish2::get_mean_incoming_inter_message_count_per_cell<Spec>(
      world
    );
    file.Update();
  }

  {
    metric = "Incoming Intra Message Cardinal Fraction";
    value = dish2::get_fraction_cardinals_incoming_intra_message<Spec>( world );
    file.Update();
  }

  {
    metric = "Incoming Intra Message Cell Fraction";
    value = dish2::get_fraction_cells_incoming_intra_message<Spec>( world );
    file.Update();
  }

  {
    metric = "Incoming Inter Message Cardinal Fraction";
    value = dish2::get_fraction_cardinals_incoming_inter_message<Spec>( world );
    file.Update();
  }

  {
    metric = "Incoming Inter Message Cell Fraction";
    value = dish2::get_fraction_cells_incoming_inter_message<Spec>( world );
    file.Update();
  }

  for (
    const auto& [k, v]
    : dish2::make_causes_of_death_string_histogram<dish2::ThreadWorld<Spec>>(
      world
    )
  ) {

    {
      metric = emp::to_string("Num Deaths ", k);
      value = v;
      file.Update();
    }

    {
      metric = emp::to_string("Fraction Deaths ", k);
      value = v / static_cast<double>( dish2::count_death_events<Spec>( world ) );
      file.Update();
    }


    {
      metric = emp::to_string("Num Deaths per Cell-update ", k);
      value = v / static_cast<double>(
        dish2::get_num_running_log_cell_updates<Spec>( world )
      );
      file.Update();
    }

    {
      metric = emp::to_string("Num Deaths per Live Cell-update ", k);
      value = v / static_cast<double>(
        dish2::get_num_running_log_live_cell_updates<Spec>( world )
      );
      file.Update();
    }

  } // end loop over death enum

  {
    metric = emp::to_string("Num Deaths per Cell-update");
    value = dish2::count_death_events<Spec>( world ) / static_cast<double>(
      dish2::get_num_running_log_cell_updates<Spec>( world )
    );
    file.Update();
  }

  {
    metric = emp::to_string("Num Deaths per Live Cell-update ");
    value = dish2::count_death_events<Spec>( world ) / static_cast<double>(
      dish2::get_num_running_log_live_cell_updates<Spec>( world )
    );
    file.Update();
  }

  {
    metric = emp::to_string("Num Births per Cell-update");
    value = dish2::count_birth_events<Spec>( world ) / static_cast<double>(
      dish2::get_num_running_log_cell_updates<Spec>( world )
    );
    file.Update();
  }

  {
    metric = emp::to_string("Num Births per Live Cell-update ");
    value = dish2::count_birth_events<Spec>( world ) / static_cast<double>(
      dish2::get_num_running_log_live_cell_updates<Spec>( world )
    );
    file.Update();
  }

  {
    metric = emp::to_string("Num Spawn Events per Cell-update");
    value = dish2::count_spawn_events<Spec>( world ) / static_cast<double>(
      dish2::get_num_running_log_cell_updates<Spec>( world )
    );
    file.Update();
  }

  {
    metric = emp::to_string("Num Spawn Events per Live Cell-update ");
    value = dish2::count_spawn_events<Spec>( world ) / static_cast<double>(
      dish2::get_num_running_log_live_cell_updates<Spec>( world )
    );
    file.Update();
  }

  for ( size_t lev{}; lev < Spec::NLEV; ++lev ) {

    const size_t num_kin_neighbors
      = dish2::count_kin_neighbors<Spec>( world, lev );
    const size_t num_neighbors = dish2::count_live_cardinals<Spec>( world );

    metric = emp::to_string("Fraction Neighbors Kin Level ", lev);
    value = num_kin_neighbors / static_cast<double>( num_neighbors );
    file.Update();

  }

  for ( size_t lev{}; lev < Spec::NLEV; ++lev ) {

    metric = emp::to_string("Maximum Kin Group Size Level ", lev);
    value = dish2::get_maximum_kin_group_size<Spec>( world, lev );
    file.Update();

  }

  for ( size_t lev{}; lev < Spec::NLEV; ++lev ) {

    metric = emp::to_string("Mean Kin Group Size Level ", lev);
    value = dish2::get_mean_kin_group_size<Spec>( world, lev );
    file.Update();

  }

  for ( size_t lev{}; lev < Spec::NLEV; ++lev ) {

    metric = emp::to_string("Median Kin Group Size Level ", lev);
    value = dish2::get_median_kin_group_size<Spec>( world, lev );
    file.Update();

  }

  for ( size_t lev{}; lev < Spec::NLEV; ++lev ) {

    metric = emp::to_string("Fraction Kin Group Loner Cells Level ", lev);
    value = dish2::get_fraction_cells_kin_group_loners<Spec>( world, lev );
    file.Update();

  }

  {
    metric = "Mean Instructions Executed per Cardinal-update";
    const size_t num_cardinal_updates
      = dish2::count_cardinals<Spec>( world ) * world.GetUpdate();
    value = dish2::sum_entire_elapsed_instruction_cycles<Spec>( world )
      / static_cast<double>( num_cardinal_updates );
    file.Update();
  }

  {
    metric = "Num Instructions Executed per Live Cardinal-update";
    const size_t num_live_cardinal_updates
      = dish2::count_live_cardinals<Spec>( world ) * world.GetUpdate();
      value = dish2::sum_entire_elapsed_instruction_cycles<Spec>( world )
        / static_cast<double>( num_live_cardinal_updates );
    file.Update();
  }

  dish2::log_msg( "wrote demographic phenotypic phylogenetic metrics" );

}

} // namespace dish2

#endif // #ifndef DISH2_RECORD_WRITE_DEMOGRAPHIC_PHENOTYPIC_PHYLOGENETIC_METRICS_HPP_INCLUDE
