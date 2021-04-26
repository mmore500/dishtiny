#pragma once
#ifndef DISH2_INTROSPECTION_SUMMARIZE_MODULE_REGULATION_HPP_INCLUDE
#define DISH2_INTROSPECTION_SUMMARIZE_MODULE_REGULATION_HPP_INCLUDE

#include <algorithm>

#include "../../../third-party/header-only-pca/include/hopca/types.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/introspection/summarize_module_regulation.hpp"

#include "../world/ThreadWorld.hpp"

#include "count_live_cardinals.hpp"
#include "get_maximum_module_count.hpp"

namespace dish2 {

template< typename Spec >
hopca::Matrix summarize_module_regulation(
  const dish2::ThreadWorld< Spec >& world
) {

  const size_t max_modules = dish2::get_maximum_module_count<Spec>( world );
  // each module potentialy has a protected and an exposed regulator
  const size_t max_regulation_dims = 2 * max_modules;
  const size_t num_live_cardinals = dish2::count_live_cardinals<Spec>( world );
  const auto& population = world.population;

  emp::vector<double> res;
  res.reserve( num_live_cardinals * max_regulation_dims );

  std::for_each(
    dish2::LiveCellIterator<Spec>::make_begin( population ),
    dish2::LiveCellIterator<Spec>::make_end( population ),
    [&]( const auto& cell ){ for (const auto& cardinal : cell) {

      auto protected_cardinal_module_regulation
        = sgpl::summarize_module_regulation(
          cardinal.cpu,
          cell.genome->program,
          0
        );
      // pad with zeros if necessary
      protected_cardinal_module_regulation.resize( max_modules );

      std::copy(
        std::begin( protected_cardinal_module_regulation ),
        std::end( protected_cardinal_module_regulation ),
        std::back_inserter( res )
      );

      auto exposed_cardinal_module_regulation
        = sgpl::summarize_module_regulation(
          cardinal.cpu,
          cell.genome->program,
          1
        );
      // pad with zeros if necessary
      exposed_cardinal_module_regulation.resize( max_modules );

      std::copy(
        std::begin( exposed_cardinal_module_regulation ),
        std::end( exposed_cardinal_module_regulation ),
        std::back_inserter( res )
      );

    } }
  );

  return hola::matrix_from_array(
    res.data(), num_live_cardinals, max_regulation_dims
  );

}

} // namespace dish2

#endif // #ifndef DISH2_INTROSPECTION_SUMMARIZE_MODULE_REGULATION_HPP_INCLUDE
