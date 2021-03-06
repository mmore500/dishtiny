#pragma once
#ifndef DISH2_LOAD_LOAD_INNOCULUM_POPULATION_HPP_INCLUDE
#define DISH2_LOAD_LOAD_INNOCULUM_POPULATION_HPP_INCLUDE

#include <algorithm>
#include <fstream>
#include <string>

#include "../../../third-party/cereal/include/cereal/archives/binary.hpp"
#include "../../../third-party/cereal/include/cereal/archives/json.hpp"
#include "../../../third-party/conduit/include/uitsl/math/math_utils.hpp"
#include "../../../third-party/conduit/include/uitsl/mpi/comm_utils.hpp"
#include "../../../third-party/conduit/include/uitsl/polyfill/filesystem.hpp"
#include "../../../third-party/Empirical/include/emp/base/always_assert.hpp"
#include "../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../third-party/Empirical/include/emp/tools/keyname_utils.hpp"
#include "../../../third-party/header-only-gzstream/include/hogzstr/gzstream.hpp"

#include "../genome/Genome.hpp"
#include "../utility/autoload.hpp"

namespace dish2 {

template< typename Spec >
emp::vector<dish2::Genome<Spec>> load_innoculum_population(
  const size_t thread_idx, const std::filesystem::path& path
) {


  const auto attrs = emp::keyname::unpack( path );

  // all innoculums must specify root id
  emp_always_assert(attrs.count("root_id"), path);

  using population_t = emp::vector<dish2::Genome<Spec>>;
  population_t innoculum(
    dish2::autoload<population_t>( path )
  );

  const size_t root_id = uitsl::stoszt( attrs.at("root_id") );
  for (auto & genome : innoculum) genome.root_id.SetID( root_id );

  if ( attrs.count("mutate_on_load") ) {
    const size_t num_muts = uitsl::stoszt( attrs.at("mutate_on_load") );
    std::cout  << "proc " << uitsl::get_proc_id() << " thread " << thread_idx
      << " applying " << num_muts << " mutations each "
      << "to population " << root_id << " from " << path << std::endl;
    for (auto& genome : innoculum) {
      for (size_t i{}; i < num_muts; ++i) genome.DoMutation();
    }
  }

  if ( attrs.count("point_mutate_on_load") ) {
    const size_t num_muts = uitsl::stoszt( attrs.at("mutate_on_load") );
    std::cout  << "proc " << uitsl::get_proc_id() << " thread " << thread_idx
      << " applying " << num_muts << " point mutations each "
      << "to population " << root_id << " from " << path << std::endl;
    for (auto& genome : innoculum) {
      for (size_t i{}; i < num_muts; ++i) genome.DoPointMutation();
    }
  }

  if ( attrs.count("insertion_mutate_on_load") ) {
    const size_t num_muts = uitsl::stoszt( attrs.at("mutate_on_load") );
    std::cout  << "proc " << uitsl::get_proc_id() << " thread " << thread_idx
      << " applying " << num_muts << " insertion mutations each "
      << "to population " << root_id << " from " << path << std::endl;
    for (auto& genome : innoculum) {
      for (size_t i{}; i < num_muts; ++i) genome.DoInsertionMutation();
    }
  }

  if ( attrs.count("deletion_mutate_on_load") ) {
    const size_t num_muts = uitsl::stoszt( attrs.at("mutate_on_load") );
    std::cout  << "proc " << uitsl::get_proc_id() << " thread " << thread_idx
      << " applying " << num_muts << " deletion mutations each "
      << "to population " << root_id << " from " << path << std::endl;
    for (auto& genome : innoculum) {
      for (size_t i{}; i < num_muts; ++i) genome.DoDeletionMutation();
    }
  }

  if ( attrs.count("set_mutation_occurence_rate_multiplicand") ) {
    const double mutation_occurence_rate_multiplicand
      = std::stod( attrs.at("set_mutation_occurence_rate_multiplicand") );
    std::cout  << "proc " << uitsl::get_proc_id() << " thread " << thread_idx
      << " setting mutation_occurence_rate_multiplicand to "
      << mutation_occurence_rate_multiplicand
      << "for population " << root_id << " from " << path << std::endl;
    for (auto& genome : innoculum) {
      genome.config_customizations.mutation_occurence_rate_multiplicand
        = mutation_occurence_rate_multiplicand;
    }
  }

  if ( attrs.count("set_program_max_size_override") ) {
    const double program_max_size_override
      = uitsl::stoszt( attrs.at("set_program_max_size_override") );
    std::cout  << "proc " << uitsl::get_proc_id() << " thread " << thread_idx
      << " setting program_max_size_override to "
      << program_max_size_override
      << "for population " << root_id << " from " << path << std::endl;
    for (auto& genome : innoculum) {
      genome.config_customizations.program_max_size_override
        = program_max_size_override;
    }
  }

  std::cout  << "proc " << uitsl::get_proc_id() << " thread " << thread_idx
    << " loaded innoculum population " << root_id
    << " with " << innoculum.size() << " genomes "
    << " from " << path << std::endl;

  return innoculum;

}

} // namespace dish2

#endif // #ifndef DISH2_LOAD_LOAD_INNOCULUM_POPULATION_HPP_INCLUDE
