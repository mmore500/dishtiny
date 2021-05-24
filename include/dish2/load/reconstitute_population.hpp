#pragma once
#ifndef DISH2_LOAD_RECONSTITUTE_POPULATION_HPP_INCLUDE
#define DISH2_LOAD_RECONSTITUTE_POPULATION_HPP_INCLUDE

#include <fstream>

#include "../../../third-party/bxzstr/include/bxzstr.hpp"
#include "../../../third-party/cereal/include/cereal/archives/binary.hpp"
#include "../../../third-party/conduit/include/uitsl/polyfill/erase_if.hpp"
#include "../../../third-party/conduit/include/uitsl/polyfill/filesystem.hpp"
#include "../../../third-party/conduit/include/uitsl/utility/keyname_directory_filter.hpp"
#include "../../../third-party/Empirical/include/emp/base/always_assert.hpp"
#include "../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../third-party/Empirical/include/emp/tools/keyname_utils.hpp"
#include "../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "../algorithm/seed_genomes_into.hpp"
#include "../debug/log_msg.hpp"
#include "../genome/Genome.hpp"
#include "../utility/autoload.hpp"
#include "../world/ThreadWorld.hpp"

namespace dish2 {

template< typename Spec >
void reconstitute_population( dish2::ThreadWorld<Spec>& world ) {

  const auto eligible_population_paths = uitsl::keyname_directory_filter({
    {"a", "population"},
    {"proc", emp::to_string( uitsl::get_proc_id() )},
    {"thread", emp::to_string( dish2::thread_idx )},
    {"ext", R"(\.bin|\.bin\.gz|\.bin\.xz|\.json|\.json\.gz|\.json\.xz)"}
  }, ".", true);

  emp_always_assert(
    eligible_population_paths.size() == 1,
    eligible_population_paths.size(), eligible_population_paths
  );

  // must set root ids here?
  auto reconstituted = dish2::autoload< emp::vector< dish2::Genome<Spec> > >(
    eligible_population_paths.front()
  );

  dish2::log_msg(
    "reconstituted ", reconstituted.size(), " cells from ",
    eligible_population_paths.front()
  );

  const auto attrs = emp::keyname::unpack( eligible_population_paths.front() );

  if ( attrs.count("filter_lowest_root") ) {
    emp_always_assert(
      attrs.at("filter_lowest_root") == "", attrs.at("filter_lowest_root")
    );

    const size_t lowest_root = std::min_element(
      std::begin( reconstituted ), std::end( reconstituted ),
      []( const auto& left_genome, const auto& right_genome ){
        return left_genome.root_id.GetID() < right_genome.root_id.GetID();
      }
    )->root_id.GetID();
    std::erase_if(
      reconstituted,
      [lowest_root]( const auto& genome ){
        return genome.root_id.GetID() != lowest_root;
      }
    );
    dish2::log_msg(
      "applied lowest_root filter, ", reconstituted.size(), " cells remain "
    );

  }

  dish2::seed_genomes_into<Spec>( reconstituted, world );

}

} // namespace dish2










#endif // #ifndef DISH2_LOAD_RECONSTITUTE_POPULATION_HPP_INCLUDE
