#pragma once
#ifndef DISH2_LOAD_APPLY_POPULATION_FILTERS_HPP_INCLUDE
#define DISH2_LOAD_APPLY_POPULATION_FILTERS_HPP_INCLUDE

#include <string>

#include "../../../third-party/conduit/include/uitsl/polyfill/erase_if.hpp"
#include "../../../third-party/Empirical/include/emp/base/always_assert.hpp"
#include "../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "../algorithm/get_lowest_root.hpp"

namespace dish2 {

template< typename Spec >
emp::vector< dish2::Genome<Spec> > apply_population_filters(
  emp::vector< dish2::Genome<Spec> > population,
  const std::string& filename
) {

  const auto attrs = emp::keyname::unpack( filename );

  if ( attrs.count("filter_lowest_root") ) {
    emp_always_assert(
      attrs.at("filter_lowest_root") == "",
      attrs.at("filter_lowest_root"), filename
    );

    const size_t lowest_root = dish2::get_lowest_root( population );
    const size_t num_erased = std::erase_if(
      population,
      [lowest_root]( const auto& genome ){
        return genome.root_id.GetID() != lowest_root;
      }
    );
    dish2::log_msg(
      "applied lowest_root filter to population from ", filename,
      ", ", num_erased, " genomes eliminated"
    );
  }

  return population;

}

} // namespace dish2

#endif // #ifndef DISH2_LOAD_APPLY_POPULATION_FILTERS_HPP_INCLUDE
