#pragma once
#ifndef DISH2_LOAD_LOAD_WORLD_HPP_INCLUDE
#define DISH2_LOAD_LOAD_WORLD_HPP_INCLUDE

#include "innoculate_population.hpp"
#include "monoculture_population.hpp"
#include "reconstitute_population.hpp"

namespace dish2 {

template< typename Spec >
void load_world( dish2::ThreadWorld<Spec>& thread_world ) {

  if ( cfg.GENESIS() == "innoculate" )
    dish2::innoculate_population( thread_world );
  else if ( cfg.GENESIS() == "monoculture" )
    dish2::monoculture_population( thread_world );
  else if ( cfg.GENESIS() == "reconstitute" )
    dish2::reconstitute_population( thread_world );
  else emp_always_assert( cfg.GENESIS() == "generate", cfg.GENESIS() );

}

} // namespace dish2

#endif // #ifndef DISH2_LOAD_LOAD_WORLD_HPP_INCLUDE
