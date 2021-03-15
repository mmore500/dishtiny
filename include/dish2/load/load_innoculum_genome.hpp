#pragma once
#ifndef DISH2_LOAD_LOAD_INNOCULUM_GENOME_HPP_INCLUDE
#define DISH2_LOAD_LOAD_INNOCULUM_GENOME_HPP_INCLUDE

#include <algorithm>
#include <fstream>
#include <string>

#include "../../../third-party/cereal/include/cereal/archives/binary.hpp"
#include "../../../third-party/cereal/include/cereal/archives/json.hpp"
#include "../../../third-party/conduit/include/uitsl/math/math_utils.hpp"
#include "../../../third-party/conduit/include/uitsl/mpi/comm_utils.hpp"
#include "../../../third-party/conduit/include/uitsl/polyfill/filesystem.hpp"
#include "../../../third-party/Empirical/include/emp/base/always_assert.hpp"
#include "../../../third-party/Empirical/include/emp/tools/keyname_utils.hpp"
#include "../../../third-party/header-only-gzstream/include/hogzstr/gzstream.hpp"

#include "../genome/Genome.hpp"
#include "../utility/autoload.hpp"

#include "innoculum_attr_handlers/_index.hpp"

namespace dish2 {

template< typename Spec >
dish2::Genome<Spec> load_innoculum_genome(
  const size_t thread_idx, const std::filesystem::path& path
) {

  const auto attrs = emp::keyname::unpack( path );

  // all innoculums must specify root id
  emp_always_assert(attrs.count("root_id"), path);

  auto innoculum = dish2::autoload<dish2::Genome<Spec>>( path );

  const size_t root_id = uitsl::stoszt( attrs.at("root_id") );
  innoculum.root_id.SetID( root_id );

  dish2::apply_mutate_on_load<Spec>( innoculum, path, thread_idx );
  dish2::apply_deletion_mutate_on_load<Spec>( innoculum, path, thread_idx );
  dish2::apply_insertion_mutate_on_load<Spec>( innoculum, path, thread_idx );
  dish2::apply_point_mutate_on_load<Spec>( innoculum, path, thread_idx );

  dish2::set_mutation_occurence_rate_multiplicand<Spec>( path, thread_idx );
  dish2::set_program_max_size_override<Spec>( path, thread_idx );

  dish2::set_intermittent_extrospective_state_exchange_probability<Spec>(
    path, thread_idx
  );
  dish2::set_intermittent_extrospective_state_rotate_probability<Spec>(
    path, thread_idx
  );
  dish2::set_extrospective_state_target_idx<Spec>( path, thread_idx );

  dish2::set_intermittent_introspective_state_exchange_probability<Spec>(
    path, thread_idx
  );
  dish2::set_intermittent_introspective_state_rotate_probability<Spec>(
    path, thread_idx
  );
  dish2::set_introspective_state_target_idx<Spec>( path, thread_idx );

  dish2::set_intermittent_writable_state_exchange_probability<Spec>(
    path, thread_idx
  );
  dish2::set_intermittent_writable_state_rotate_probability<Spec>(
    path, thread_idx
  );
  dish2::set_writable_state_target_idx<Spec>( path, thread_idx );

  std::cout  << "proc " << uitsl::get_proc_id() << " thread " << thread_idx
    << " loaded innoculum genome " << root_id << " from " << path << std::endl;

  return innoculum;

}

} // namespace dish2

#endif // #ifndef DISH2_LOAD_LOAD_INNOCULUM_GENOME_HPP_INCLUDE
