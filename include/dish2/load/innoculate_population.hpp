#pragma once
#ifndef DISH2_LOAD_INNOCULATE_POPULATION_HPP_INCLUDE
#define DISH2_LOAD_INNOCULATE_POPULATION_HPP_INCLUDE

#include <algorithm>
#include <set>

#include "../../../third-party/conduit/include/uitsl/polyfill/filesystem.hpp"
#include "../../../third-party/conduit/include/uitsl/utility/keyname_directory_filter.hpp"
#include "../../../third-party/Empirical/include/emp/base/always_assert.hpp"
#include "../../../third-party/Empirical/include/emp/base/assert.hpp"
#include "../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../third-party/Empirical/include/emp/tools/keyname_utils.hpp"
#include "../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "../algorithm/seed_genomes_into.hpp"
#include "../debug/log_msg.hpp"
#include "../genome/Genome.hpp"
#include "../world/ThreadWorld.hpp"

#include "dose_innoculum_buckets.hpp"
#include "load_innoculum.hpp"
#include "read_innoculum_doses.hpp"

namespace dish2 {

template< typename Spec >
void innoculate_population( dish2::ThreadWorld<Spec>& world ) {

  const auto innoculum_paths = uitsl::keyname_directory_filter({
    {"a", "genome|population"},
    {"root_id", "[0-9]+"},
    {"ext", R"(\.bin|\.bin\.gz|\.bin\.xz|\.json|\.json\.gz|\.json\.xz)"}
  }, ".", true);

  emp::vector<emp::vector<dish2::Genome<Spec>>> innoculum_buckets;
  std::transform(
    std::begin( innoculum_paths ),
    std::end( innoculum_paths ),
    std::back_inserter( innoculum_buckets ),
    [](const auto& path){
      return dish2::load_innoculum<Spec>(path);
    }
  );

  // we should have at least one bucket
  emp_always_assert( innoculum_buckets.size() );

  // no buckets should be empty
  emp_always_assert( std::all_of(
    std::begin( innoculum_buckets ), std::end( innoculum_buckets ),
    []( const auto& bucket ){ return bucket.size(); }
  ) );

  // all genomes within a bucket have the same root id
  emp_assert([&](){
    for ( const auto& bucket : innoculum_buckets ) if ( std::adjacent_find(
      std::begin( bucket ), std::end( bucket ),
      []( const auto& left, const auto& right ){
        return left.root_id.GetID() != right.root_id.GetID();
      }
    ) != std::end(bucket) ) return false;
    return true;
  }(), emp::to_string( innoculum_paths ));

  // all buckets have unique root ids
  emp_always_assert([&](){
    std::set<size_t> root_ids;
    std::transform(
      std::begin( innoculum_buckets ), std::end( innoculum_buckets ),
      std::inserter(root_ids, std::begin(root_ids)),
      []( const auto& bucket ){ return bucket.front().root_id.GetID(); }
    );
    return root_ids.size() == innoculum_buckets.size();
  }(), emp::to_string( innoculum_paths ));

  const emp::vector<size_t> innoculum_doses = dish2::read_innoculum_doses(
    innoculum_paths
  );

  // don't run subroutine if no innoculum doses adjusted
  // to minimize subtle runtime effects on existing code
  if (std::any_of(
    std::begin(innoculum_doses), std::end(innoculum_doses),
    [](const size_t dose) { return dose != 1; }
  )) {
    dish2::log_msg(
      innoculum_buckets.size(), " innoculum buckets before dosing"
    );
    innoculum_buckets = dish2::dose_innoculum_buckets(
      innoculum_buckets,
      innoculum_doses
    );
    dish2::log_msg(
      innoculum_buckets.size(), " innoculum buckets after dosing"
    );
  }

  dish2::seed_genomes_into<Spec, true>( innoculum_buckets, world );

  dish2::log_msg( "loaded ", innoculum_buckets.size(), " innoculum buckets" );

}

} // namespace dish2

#endif // #ifndef DISH2_LOAD_INNOCULATE_POPULATION_HPP_INCLUDE
