#pragma once
#ifndef DISH2_INTROSPECTION_WRITE_SUMMARY_METRICS_HPP_INCLUDE
#define DISH2_INTROSPECTION_WRITE_SUMMARY_METRICS_HPP_INCLUDE

#include <cstdlib>
#include <mutex>

#include "../../../third-party/Empirical/include/emp/base/macros.hpp"
#include "../../../third-party/Empirical/include/emp/data/DataFile.hpp"
#include "../../../third-party/Empirical/include/emp/tools/keyname_utils.hpp"

#include "count_dead_cells.hpp"
#include "count_live_cells.hpp"
#include "count_unique_coding_genotypes.hpp"
#include "count_unique_root_ids.hpp"
#include "get_mean_elapsed_insertions_deletions.hpp"
#include "get_mean_elapsed_point_mutations.hpp"
#include "get_mean_epoch.hpp"
#include "get_mean_generation.hpp"
#include "get_mean_module_count.hpp"
#include "get_mean_program_length.hpp"
#include "get_prevalent_coding_genotype.hpp"

namespace dish2 {

template< typename Spec >
void write_summary_metrics(
  const dish2::ThreadWorld< Spec >& world, const size_t thread_id
) {

  thread_local emp::DataFile file( emp::keyname::pack({
    {"a", "summary_metrics"},
    {"repro", std::getenv("REPRO_ID") ?: ""},
    {"source", EMP_STRINGIFY(DISHTINY_HASH_)},
    {"thread", emp::to_string(thread_id)},
    {"ext", ".csv"}
  }) );

  thread_local std::string metric;
  thread_local double value;

  thread_local std::once_flag once_flag;
  std::call_once(once_flag, [](){
    file.AddVar(metric, "Metric");
    file.AddVar(value, "Value");
    file.PrintHeaderKeys();
  });

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
    metric = "Number Unique Genotypes";
    value = dish2::count_unique_coding_genotypes<Spec>( world );
    file.Update();
  }

  {
    metric = "Number Phylogenetic Roots";
    value = dish2::count_unique_root_ids<Spec>( world );
    file.Update();
  }

  {
    metric = "Mean Elapsed Insertion/Deletion Mutations";
    value = dish2::get_mean_elapsed_insertions_deletions<Spec>( world );
    file.Update();
  }

  {
    metric = "Mean Elapsed Point Mutations";
    value = dish2::get_mean_elapsed_point_mutations<Spec>( world );
    file.Update();
  }

  {
    metric = "Mean Current Epoch";
    value = dish2::get_mean_epoch<Spec>( world );
    file.Update();
  }

  for (size_t lev{}; lev <= Spec::NLEV; ++lev) {
    metric = emp::to_string("Mean Elapsed Generations Level ", lev);
    value = dish2::get_mean_generation<Spec>( world, lev );
    file.Update();
  }

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
    metric = "Random Number Generator Seed";
    value = sgpl::ThreadLocalRandom::Get().GetSeed();
    file.Update();
  }

}

} // namespace dish2

#endif // #ifndef DISH2_INTROSPECTION_WRITE_SUMMARY_METRICS_HPP_INCLUDE
