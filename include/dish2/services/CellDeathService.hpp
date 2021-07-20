#pragma once
#ifndef DISH2_SERVICES_CELLDEATHSERVICE_HPP_INCLUDE
#define DISH2_SERVICES_CELLDEATHSERVICE_HPP_INCLUDE

#include <algorithm>

#include "../../../third-party/conduit/include/uitsl/math/shift_mod.hpp"
#include "../../../third-party/conduit/include/uitsl/polyfill/identity.hpp"

#include "../cell/cardinal_iterators/CellAgeWrapper.hpp"
#include "../cell/cardinal_iterators/GenomeNodeOutputWrapper.hpp"
#include "../cell/cardinal_iterators/NeighborIsAliveWrapper.hpp"
#include "../config/cfg.hpp"
#include "../debug/LogScope.hpp"
#include "../enum/CauseOfDeath.hpp"

namespace dish2 {

/**
 * If cell age exceeds CELL_AGE_DURATION, cull it.
 */
class CellDeathService {

  template<typename Cell>
  static void DoCull( Cell& cell ) {

    using spec_t = typename Cell::spec_t;

    // if cell has no live neighbors, send one birth request to a neighbor
    if ( std::none_of(
      cell.template begin<dish2::NeighborIsAliveWrapper<spec_t>>(),
      cell.template end<dish2::NeighborIsAliveWrapper<spec_t>>(),
      std::identity
    ) ) {
      cell.template begin<dish2::GenomeNodeOutputWrapper<spec_t>>()->TryPut(
        *cell.genome
      );
    }

    cell.DeathRoutine( dish2::CauseOfDeath::age );

  }

public:

  static bool ShouldRun( const size_t update, const bool alive ) {
    const size_t freq = dish2::cfg.CELL_AGE_SERVICE_FREQUENCY();
    return
      alive
      && freq > 0
      && uitsl::shift_mod( update, freq ) == 0;
  }

  template<typename Cell>
  static void DoService( Cell& cell ) {

    const dish2::LogScope guard{ "cell death service", "TODO", 3 };

    using spec_t = typename Cell::spec_t;

    // check cell age consistency
    emp_assert((
      std::set<typename dish2::CellAgeWrapper<spec_t>::value_type>(
        cell.template begin<dish2::CellAgeWrapper<spec_t>>(),
        cell.template end<dish2::CellAgeWrapper<spec_t>>()
      ).size() == 1
    ));

    if (
      *cell.template begin<dish2::CellAgeWrapper<spec_t>>()
      > cfg.CELL_AGE_DURATION()
    ) DoCull<Cell>( cell );

  }

};

} // namespace dish2

#endif // #ifndef DISH2_SERVICES_CELLDEATHSERVICE_HPP_INCLUDE
