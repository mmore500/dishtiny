#pragma once
#ifndef DISH2_INTROSPECTION_MAKE_CAUSES_OF_DEATH_STRING_HISTOGRAM_HPP_INCLUDE
#define DISH2_INTROSPECTION_MAKE_CAUSES_OF_DEATH_STRING_HISTOGRAM_HPP_INCLUDE

#include <algorithm>
#include <string>

#include "../../../third-party/Empirical/include/emp/tools/string_utils.hpp"
#include "../../../third-party/magic_enum/include/magic_enum.hpp"

#include "../enum/CauseOfDeath.hpp"
#include "../utility/make_histogram.hpp"

#include "summarize_causes_of_death.hpp"

namespace dish2 {

template< typename ThreadWorld >
auto make_causes_of_death_string_histogram( const ThreadWorld& world ) {

  const auto summary = dish2::summarize_causes_of_death<ThreadWorld>( world );

  auto res = dish2::make_histogram(
    std::begin( summary ), std::end( summary ),
    []( const dish2::CauseOfDeath val ){ return emp::to_string( val ); }
  );

  // ensure all keys are represented
  for (const auto val : magic_enum::enum_values<dish2::CauseOfDeath>()) {
    res[ std::string{ magic_enum::enum_name( val ) } ];
  }

  return res;

}

} // namespace dish2

#endif // #ifndef DISH2_INTROSPECTION_MAKE_CAUSES_OF_DEATH_STRING_HISTOGRAM_HPP_INCLUDE
