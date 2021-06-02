#pragma once
#ifndef DISH2_LOAD_RECONSTITUTE_POPULATION_HPP_INCLUDE
#define DISH2_LOAD_RECONSTITUTE_POPULATION_HPP_INCLUDE

#include <fstream>

#include "../algorithm/seed_genomes_into.hpp"
#include "../debug/log_msg.hpp"
#include "../utility/is_native.hpp"
#include "../world/ThreadWorld.hpp"

#include "apply_population_filters.hpp"
#include "reconstitute_population_load.hpp"

namespace dish2 {

template< typename Spec >
void reconstitute_population( dish2::ThreadWorld<Spec>& world ) {

  const auto [src, reconstituted]
    = dish2::reconstitute_population_load<Spec>();

  dish2::log_msg("reconstituted ", reconstituted.size(), " cells from ", src);

  dish2::seed_genomes_into<Spec, dish2::is_native()>( reconstituted, world );

}

} // namespace dish2










#endif // #ifndef DISH2_LOAD_RECONSTITUTE_POPULATION_HPP_INCLUDE
